#include <iostream>
#include "sort.hpp"
#include "vector.hpp"
#include "pair_lab1.hpp"

int main() {
    TVector< TPair<int, std::string> > input;
    int key;
    std::string value;
    while (std::cin >> key >> value) {
        //std::cout << key << " " << value << std::endl;
        input.insert(TPair<int, std::string>(key, value), 0);
        //std::cout << input.getSize() << std::endl;
    }

    sort::counting_sort(&input);
    int num_count = input.getSize();

    //Вывод
    TPair<int, std::string> buff;
    for (int i = 0; i < num_count; ++i) {
        buff = input.at(i);
        printf("%06d\t%s\n", buff.first, buff.second.c_str());
    }

    return 0;
}
