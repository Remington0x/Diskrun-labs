#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>

struct TNode {
    int Position;
    int Key;
    std::string Data;
    struct TNode* Next;
};

int VectorPushBack(struct TNode* root, int key, std::string data) {
    while (root->Next != NULL) {
        root = root->Next;
    }
    root->Next = (struct TNode*)malloc(sizeof(struct TNode));
    if (root->Next != NULL) {
        root = root->Next;
        root->Position = 0;
        root->Key = key;
        root->Data = data;
        root->Next = NULL;
    } else {
        return -1;
    }

    return 0;
}

int

// class TVector {
// private:
//     unsigned int Size;
//     struct TNode Vector;
// public:
//     TVector() {
//
//     };
// };
