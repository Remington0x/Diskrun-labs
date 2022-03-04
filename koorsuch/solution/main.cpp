#include "tree.hpp"

int main() {
    TTree* tree = new TTree();
    char op;
    double buff;
    std::cout << "a -- add key\nd -- delete key\np -- print tree\n";
    while (std::cin >> op) {
        switch (op)
        {
        case 'a':
            std::cin >> buff;
            tree->AddNode(buff);
            break;

        case 'd':
            std::cin >> buff;
            tree->RemNode(buff);
            break;

        case 'p':
            std::cout << "There are " << tree->Roots.size() << " roots. Which do you want to print?\n";
            std::cin >> buff;
            tree->PrintTree((int)buff);
            break;
        
        default:
            break;
        }
    }
}