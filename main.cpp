/*
 * Exercise 2
 * TDT4171 Artificial Intelligence Methods
 *
 * Solution by Martin T. Sandsmark <martin.sandsmark@kde.org>
 *
 * Requirements:
 *  • Eigen 2.0 or later (included locally for your convenience)
 *  • A C++-compiler (tested with GCC 4.4.3)
 */

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vector>

// Import most common Eigen types into our namespace
USING_PART_OF_NAMESPACE_EIGEN;

// Hasslefree (TM)
using std::cout;
using std::endl;
using std::vector;

/**
  * Recursive implementation of the Forward formula.
  */
Vector2f forward(vector<Matrix2f> observed, Matrix2f transitions, int t)
{
    if (t == 0) { // Initial condition
        Vector2f f;
        f << 0.5, 0.5;
        return f;
    }
    Vector2f f = observed.at(t) * transitions.transpose() * forward(observed, transitions, t-1);
    f = f / f.sum(); // "Normalize" the variables, so that the sum is 1.0 (not normal matrix normalization).
    cout << "Normalized forward message day " << t << ":\n" << f << endl;
    return f;
}

/**
  * Stupid and inefficient implementation of the Forward-Backward algorithm
  */
Vector2f forwardBackward(vector<Matrix2f> observed, Matrix2f transitions, int totalDays, int day)
{
    vector<Vector2f> fv; // A vector of forward messages
    vector<Vector2f> sv; // A vector with smoothed estimates

    Vector2f b; // Backwards message
    b << 1.0, 1.0; // Initial value for backwards message
    
    Vector2f f; // Initial condition
    f << 0.5, 0.5;
    fv.push_back(f); // Add the initial condition to the top of our stack of conditions

    // Iterative implementation of the Forward formula
    for (int i=1; i<=totalDays; i++) {
        fv.push_back(observed[i] * transitions * fv[i-1]);
    }

    Vector2f temp; // Holds the newly smoothed estimate
    for (int i=totalDays; i>=0; i--) { // Backward
        cout << "Normalized forward message (b) for k  " << i << ":\n" << b << endl;
        temp[0] = fv[i][0] * b[0]; //
        temp[1] = fv[i][1] * b[1]; // Smooth
        temp /= temp.sum(); // "Normalize"
        sv.insert(sv.begin(), temp); // Insert into our list of calculated smoothed estimates
        b = transitions * observed[i] * b; // Calculate new backwards estimate
        b /= b.sum(); // "Normalize" backwards estimate
    }

    // Return the wanted day
    return sv[day];
}

int main(int, char *[])
{
    // Matrix containing transitional probabilities
    Matrix2f transitions;
    transitions << 0.7, 0.3, //
                   0.3, 0.7; // From page 540 in Russel&Norvig
    
    // Matrix containing conditional probabilities
    Matrix2f conditional;
    conditional << 0.9, 0.1, //
                   0.2, 0.8; // Shamelessly stolen from the book

    // Store up pre-defined observations, shared by all part-tasks, luckily
    vector<Matrix2f> observed;
    Matrix2f o;
    o << 0.9, 0.0,         //
         0.0, 0.2;         // Garbage values because the algorithm in the book is 1-indexed, and it is getting late.
    observed.push_back(o); //
    o << 0.9, 0.0,
         0.0, 0.2;
    observed.push_back(o);
    o << 0.9, 0.0,
         0.0, 0.2;
    observed.push_back(o);
    o << 0.1, 0.0,
         0.0, 0.8;
    observed.push_back(o);
    o << 0.9, 0.0,
         0.0, 0.2;
    observed.push_back(o);
    o << 0.9, 0.0,
         0.0, 0.2;
    observed.push_back(o);
    
    Vector2f b1 = forward(observed, transitions, 2);
    cout << "B1: Probability of rain day two, given umbrella was worn during day 1 and 2:\n" << b1[0] << "\n" << endl;

    Vector2f b2 = forward(observed, transitions, 5);
    cout << "B2: Probability of rain day 5, given evidence from PartB2 in the assignment:\n" << b2[0] << "\n" << endl;

    Vector2f c1 = forwardBackward(observed, transitions, 2, 2);
    cout << "C1: \n" << c1 << "\n" << endl;

    Vector2f c2 = forwardBackward(observed, transitions, 5, 1);
    cout << "C2: Probability of rain at day 1: \n" << c2[0] << "\n" << endl;
}

