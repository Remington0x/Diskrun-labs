FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp tree.hpp
	g++ $(FLAGS) main.cpp -o solution

clean:
	rm -rf *.o solution
