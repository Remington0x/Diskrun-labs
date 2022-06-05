FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp tree.hpp
	g++ $(FLAGS) main.cpp -o solution

naive: super_naive.cpp
	g++ $(FLAGS) super_naive.cpp -o naive

gen: gen.cpp
	g++ $(FLAGS) gen.cpp -o gen

smart: tree_sp.hpp main.cpp
	g++ $(FLAGS) main.cpp -o smart

clean:
	rm -rf *.o solution
