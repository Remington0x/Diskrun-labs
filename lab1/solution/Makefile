FLAGS=-std=c++14 -g -pedantic -Wall -Wextra -Wno-unused-variable
all: solution

solution: lab1.cpp
	g++ $(FLAGS) lab1.cpp -o solution

bench: bench.cpp
	g++ $(FLAGS) bench.cpp -o bench

clean:
	rm -rf solution
