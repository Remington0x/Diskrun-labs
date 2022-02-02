FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp suffix_tree.hpp
	g++ $(FLAGS) main.cpp -o solution

naive: naive.cpp
	g++ $(FLAGS) naive.cpp -o naive

clean:
	rm -rf *.o solution