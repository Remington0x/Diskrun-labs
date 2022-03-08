#include <iostream>
#include <cstdlib>

//const int N = 50;
//const int M = 50;

int main() {
    int val;
    std::cin >> val;
    int M = val;
    int N = val;
    std::cin >> val;
    std::srand(val);
    std::cout << N << " " << M << std::endl;
    for (int i = 0; i < N; ++i) {
        val = std::rand() % 10000;
        std::cout << val << " " << val + std::rand() % 1000 << " " << std::rand() % 10000 << std::endl;
    }
    for (int i = 0; i < M; ++i) {
        std::cout << std::rand() % 10000 << " " << std::rand() % 10000 << std::endl;
    }

    return 0;
}