CXX = clang++
CXXFLAGS = -std=c++11

all: main.o tester.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o*
