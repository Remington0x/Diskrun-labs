#include "tree.hpp"

#include <algorithm>

struct TStrip {
    double XLeft;
    TNode* TreeRoot;
    TStrip(double l, TNode* root) {
        XLeft = l;
        TreeRoot = root;
    }
    ~TStrip() {}
};

struct TSegment {
    double L;
    double R;
    double H;
    TSegment(double left, double right, double height) {
        L = left;
        R = right;
        H = height;
    }
    ~TSegment() {}
    bool operator < (const TSegment & b) {
        return this->L < b.L;
    }
};

int main() {
    TTree* tree = new TTree();
    std::vector<TStrip> strips;
    std::vector<TSegment> vec;
    std::vector<TSegment> queue;

    int n, m;
    double l, r, h;
    double x, y;

    std::cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        std::cin >> l >> r >> h;
        vec.push_back(TSegment(l, r, h));
    }
    std::sort(vec.begin(), vec.end());

    //vot ono -- chudo detorozhdeniya
    TNode* newRoot;
    
    while (!vec.empty()) {
        if (!queue.empty()) {
            if (queue.at(0).R < vec.at(0).L) {  //segment queue[0] ends
                newRoot = tree->RemNode(queue.at(0).H);
                strips.push_back(TStrip(queue.at(0).R, newRoot));
                queue.erase(queue.begin());
            } else 
            if (queue.at(0).R > vec.at(0).L) {  //segment vec[0] starts
                newRoot = tree->AddNode(vec.at(0).H);
                strips.push_back(TStrip(vec.at(0).L, newRoot));
                vec.erase(vec.begin());
            } else {    //queue[0] ends and vec[0] starts
                tree->RemNode(queue.at(0).H);
                newRoot = tree->AddNode(vec.at(0).H);
                strips.push_back(TStrip(vec.at(0).L, newRoot));
                vec.erase(vec.begin());
                queue.erase(queue.begin());
            }
        } else {    //queue is empty
            newRoot = tree->AddNode(vec.at(0).H);
            strips.push_back(TStrip(vec.at(0).L, newRoot));
            vec.erase(vec.begin());
        }
    }

    //ready to query?

    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        //1. determine strip
        
        //2. determine branch of tree

    }






    // char op;
    // double buff;
    // std::cout << "a -- add key\nd -- delete key\np -- print tree\n";
    // while (std::cin >> op) {
    //     switch (op)
    //     {
    //     case 'a':
    //         std::cin >> buff;
    //         tree->AddNode(buff);
    //         break;

    //     case 'd':
    //         std::cin >> buff;
    //         tree->RemNode(buff);
    //         break;

    //     case 'p':
    //         std::cout << "There are " << tree->Roots.size() << " roots. Which do you want to print?\n";
    //         std::cin >> buff;
    //         tree->PrintTree((int)buff);
    //         break;
        
    //     default:
    //         break;
    //     }
    // }
}