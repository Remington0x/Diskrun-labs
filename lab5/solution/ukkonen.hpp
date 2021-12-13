#ifndef UKKONEN_HPP
#define UKKONEN_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

class TSuffTree {
public:
    TSuffTree() {
        root = new TNode();
        endPos = std::make_shared<unsigned int>(0);
    }

    ~TSuffTree() {
        RecDelete(root);
    }

    void Fill(std::string);
    void Print();
private:
    struct TNode {
        bool isLeaf;
        int l;
        std::shared_ptr<unsigned int> r;
        std::map<char, TNode*> to;
    };

    TNode* root;
    std::string string;
    std::shared_ptr<unsigned int> endPos;

    void RecDelete(TNode*);
    void RecPrint(TNode*, int);
};

void TSuffTree::Fill(std::string s) {
    string = s;
    TNode* curNode = root;
    char curEdge;
    int curLength;
    int newSuffCount = 1;

    for (*endPos = 0; *endPos < s.length(); ++(*endPos)) {
        if (curNode->to.find(s[*endPos]) == curNode->to.end()) {
            newSuffCount = 1;
            TNode* newNode = new TNode();
            newNode->l = *endPos;
            newNode->r = endPos;
            curNode->to[s[*endPos]] = newNode;
        } else {
            curNode = root;
            curEdge = s[*endPos];
            curLength = 1;
            ++newSuffCount;
        }
    }
}

void TSuffTree::Print() {
    RecPrint(root, 0);
}

void TSuffTree::RecPrint(TNode* node, int tab) {
    for (auto it: node->to) {
        RecPrint(it.second, tab + 1);
    }
    if (node != root) {
        for (int i = 0; i < tab; ++i) {
            std::cout << '\t';
        }
        for (unsigned int i = node->l; i <= *node->r; ++i) {
            std::cout << string[i];
        }
        std::cout << std::endl;
    }
}

void TSuffTree::RecDelete(TNode* node) {
    for (auto it: node->to) {
        RecDelete(it.second);
    }
    delete node;
}

#endif