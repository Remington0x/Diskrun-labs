FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp
	g++ $(FLAGS) main.cpp -o solution

bench: bench.cpp
	g++ $(FLAGS) bench.cpp -o bench

clean:
	rm -rf *.o solution bench