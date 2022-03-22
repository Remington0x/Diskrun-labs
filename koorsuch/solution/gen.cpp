#include <iostream>
#include <cstdlib>

//const int N = 50;
//const int M = 50;

int main() {
    int val1, val2;
    std::cin >> val1 >> val2;
    int N = val1;
    int M = val2;
    std::cin >> val1;
    std::srand(val1);
    std::cout << N << " " << M << std::endl;
    for (int i = 0; i < N; ++i) {
        val1 = std::rand() % 10000;
        std::cout << val1 << " " << val1 + std::rand() % 1000 << " " << std::rand() % 10000 << std::endl;
    }
    for (int i = 0; i < M; ++i) {
        std::cout << std::rand() % 10000 << " " << std::rand() % 10000 << std::endl;
    }

    return 0;
}