FLAGS=-std=c++14 -pedantic -g -Wall -Wextra -Wno-unused-variable
all: solution

solution: lab2-nocomms.cpp
	g++ $(FLAGS) lab2-nocomms.cpp -o solution
	
test: tester.cpp
	g++ $(FLAGS) tester.cpp -o tester

profiling: lab2.cpp
	g++ $(FLAGS) lab2.cpp -o solution -pg

clean:
	rm -rf solution tester test.txt
