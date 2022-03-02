FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp
	g++ $(FLAGS) main.cpp -o solution

warshell: warshell.cpp
	g++ $(FLAGS) warshell.cpp -o warshell

clean:
	rm -rf *.o solution