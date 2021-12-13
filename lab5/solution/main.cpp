#include <iostream>
#include "ukkonen.hpp"

int main() {
    TSuffTree* tree = new TSuffTree();

    std::string s;
    std::cin >> s;
    tree->Fill(s);
    tree->Print();

    delete tree;
    return 0;
}