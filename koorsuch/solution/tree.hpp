#ifndef TREE_HPP
#define TREE_HPP

/*Получается у нас есть вектор указателей на корни деревьев. 

*/

#include <iostream>
#include <vector>

struct TNode {
    TNode* Left;
    TNode* Right;
    double Height;
    TNode() {
        Left = nullptr;
        Right = nullptr;
    };
    TNode(double h) {
        Height = h;
        Left = nullptr;
        Right = nullptr;
    }
    ~TNode() {}
};

class TTree {
public:
    TNode* Root;
    TTree() {
        Root = new TNode();
    }
    TTree(double h) {
        Root = new TNode(h);
    }

    TNode* AddNode(double);
    TNode* RemNode(double);
};

TNode* TTree::AddNode(double h) {
    //TNode* newNode = new TNode(h);
    TNode* newRoot = new TNode(Root->Height);
    TNode* curNewNode = newRoot;
    TNode* curNode = Root;
    bool flag = true;

    while (flag) {
        if (h < curNode->Height) {
            curNewNode->Right = curNode->Right;
            if (curNode->Left != nullptr) {
                curNewNode->Left = new TNode(curNode->Left->Height);
                curNewNode = curNewNode->Left;
                curNode = curNode->Left;
            } else {
                curNewNode->Left = new TNode(h);
                flag = false;
            }
        } else {
            curNewNode->Left = curNode->Left;
            if (curNode->Right != nullptr) {
                curNewNode->Right = new TNode(curNode->Right->Height);
                curNewNode = curNewNode->Right;
                curNode = curNode->Right;
            } else {
                curNewNode->Right = new TNode(h);
                flag = false;
            }
        }
    }

    return newRoot;
}

TNode* TTree::RemNode(double h) {
    TNode* newRoot = new TNode(Root->Height);
    TNode* curNewNode = newRoot;
    TNode* curNode = Root;
    TNode* parent = newRoot;
    bool flag = true;

    while (flag) {
        if (h < curNode->Height) {
            curNewNode->Right = curNode->Right;
            if (curNode->Left != nullptr) {
                curNewNode->Left = new TNode(curNode->Left->Height);
                parent = curNewNode;
                curNewNode = curNewNode->Left;
                curNode = curNode->Left;
            } else {
                flag = false;
                throw std::logic_error("No such element in the tree");
            }
        } else 
        if (h > curNode->Height) {
            curNewNode->Left = curNode->Left;
            if (curNode->Right != nullptr) {
                curNewNode->Right = new TNode(curNode->Right->Height);
                parent = curNewNode;
                curNewNode = curNewNode->Right;
                curNode = curNode->Right;
            } else {
                flag = false;
                throw std::logic_error("No such element in the tree");
            }
        } else {
            //three cases:
            //1. no children
            //2. 1 child
            //3. 2 children
            if (curNode->Left == nullptr) {     //there is no left child
                if (curNode->Right == nullptr) {//threre are no children
                    if (curNode == Root) {
                        delete newRoot;
                        return nullptr;
                    }
                    delete curNewNode;
                    if (curNewNode == parent->Left) {
                        parent->Left = nullptr;
                    } else {
                        parent->Right = nullptr;
                    }
                } else {    //there is right child
                    if (curNode == Root) {
                        delete newRoot;
                        newRoot = curNode->Right;
                    }
                    if (curNewNode == parent->Left) {
                        parent->Left = curNode->Right;
                        delete curNewNode;
                    } else {
                        parent->Right = curNode->Right;
                        delete curNewNode;
                    }
                }
            } else {
                if (curNode->Right == nullptr) {//there is only left child
                    if (curNode == Root) {
                        delete newRoot;
                        newRoot = curNode->Left;
                    }
                    if (curNewNode == parent->Left) {
                        parent->Left = curNode->Left;
                        delete curNewNode;
                    } else {
                        parent->Right = curNode->Left;
                        delete curNewNode;
                    }
                } else {    //there are both children
                    //go to the right
                    //walk through the left subtree copying all the nodes
                    //when the leaf is reached, swap cur and leaf
                    TNode* buffNode = curNode->Right;
                    TNode* buffNewNode = new TNode(curNode->Right->Height);
                    curNewNode->Left = curNode->Left;

                    if (curNode == Root) {
                        

                        delete newRoot;
                        newRoot = curNode->Right;
                    }

                    while (buffNode->Left->Left != nullptr) {
                        buffNewNode->Right = buffNode->Right;
                        buffNewNode->Left = new TNode(buffNode->Left->Height);
                        buffNode = buffNode->Left;
                        buffNewNode = buffNewNode->Left;
                    }
                    //at this point buffNode->left is nullptr

                }
            }

        }
    }
}



#endif