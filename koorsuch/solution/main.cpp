#include "tree.hpp"

struct TStrip {
    double XRight;
    TNode* TreeRoot;
};

int main() {
    TTree* tree = new TTree();
    std::vector<TStrip> strips;

    int n, m;
    double l, r, h;
    double x, y;

     std::cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        std::cin >> l >> r >> h;
        //idk im frustrated
    }











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