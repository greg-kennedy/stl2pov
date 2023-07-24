all: stl2pov

clean:
	rm -f *.o stl2pov

stl2pov: stl2pov.cpp
	c++ -std=c++20 -O3 -Wall -flto -s -o stl2pov stl2pov.cpp
