FLAGS = -std=c++17 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: main.cpp
	g++ $(FLAGS) main.cpp -o solution

draft: draft.cpp
	g++ $(FLAGS) draft.cpp -o draft

clean:
	rm -rf *.o solution