assignment2: clean
	g++ -I/usr/include/eigen2 -I. -g -o assignment2 main.cpp

clean:
	rm -f assignment2
