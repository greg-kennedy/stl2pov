CXX ?= c++

# optimization flags
CFLAGS += -O3 -march=native -flto -s

all: stl2pov

clean:
	rm -f *.o stl2pov

stl2pov: stl2pov.cpp
	$(CXX) -std=c++11 -fno-rtti -fno-exceptions -Wall -Wextra $(CFLAGS) -o stl2pov stl2pov.cpp
