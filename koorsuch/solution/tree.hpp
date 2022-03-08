#ifndef TREE_HPP
#define TREE_HPP


#include <iostream>
#include <vector>

struct TNode {
    TNode* Left;
    TNode* Right;
    double Height;
    // TNode() {
    //     Left = nullptr;
    //     Right = nullptr;
    // };
    TNode(double h) {
        Height = h;
        Left = nullptr;
        Right = nullptr;
    }
    ~TNode() {}
};

class TTree {
public:
    std::vector<TNode*> Roots;
    TTree() {}
    ~TTree() {}

    void PrintTree(int);
    TNode* AddNode(double);
    TNode* RemNode(double);

private:
    void RecPrintTree(TNode*, int);
};

void TTree::RecPrintTree(TNode* node, int tab) {
    if (node->Right != nullptr) {
        RecPrintTree(node->Right, tab + 1);
    }
    for (int i = 0; i < tab; ++i)
        std::cout << "  ";
    std::cout << node->Height << std::endl;
    if (node->Left != nullptr) {
        RecPrintTree(node->Left, tab + 1);
    }
}

void TTree::PrintTree(int rootNum) {
    RecPrintTree(Roots.at(rootNum), 0);
}

TNode* TTree::AddNode(double h) {
    TNode* Root;
    TNode* newRoot;
    if (Roots.empty() || Roots.back() == nullptr) {
        TNode* newRoot = new TNode(h);
        Roots.push_back(newRoot);
        return newRoot;
    } else {
        Root = Roots.back();
        newRoot = new TNode(Root->Height);
    }
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

    Roots.push_back(newRoot);
    return newRoot;
}

TNode* TTree::RemNode(double h) {
    TNode* Root;
    if (Roots.empty()) {
        throw std::logic_error("Attempting to delete from empty tree");
    }
    Root = Roots.back();
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
                        newRoot = nullptr;
                    } else {
                        delete curNewNode;
                        if (curNewNode == parent->Left) {
                            parent->Left = nullptr;
                        } else {
                            parent->Right = nullptr;
                        }
                    }
                    flag = false;
                } else {    //there is right child
                    if (curNode == Root) {
                        delete newRoot;
                        newRoot = curNode->Right;
                    } else {
                        if (curNewNode == parent->Left) {
                            parent->Left = curNode->Right;
                            delete curNewNode;
                        } else {
                            parent->Right = curNode->Right;
                            delete curNewNode;
                        }
                    }
                    flag = false;
                }
            } else {
                if (curNode->Right == nullptr) {//there is only left child
                    if (curNode == Root) {
                        delete newRoot;
                        newRoot = curNode->Left;
                    } else {
                        if (curNewNode == parent->Left) {
                            parent->Left = curNode->Left;
                            delete curNewNode;
                        } else {
                            parent->Right = curNode->Left;
                            delete curNewNode;
                        }
                    }
                    flag = false;
                } else {    //there are both children
                    //go to the right
                    //walk through the left subtree copying all the nodes
                    //when the leaf is reached, swap cur and leaf
                    TNode* buffNode = curNode->Left;
                    TNode* buffNewNode = new TNode(curNode->Left->Height);
                    curNewNode->Left = buffNewNode;

                    if (curNode == Root) {
                        if (buffNode->Right == nullptr) {
                            newRoot->Height = curNode->Left->Height;
                            newRoot->Right = Root->Right;
                            newRoot->Left = curNode->Left->Left;
                            delete buffNewNode;
                            flag = false;
                        } else {
                            newRoot->Left = buffNewNode;
                            while (buffNode->Right->Right != nullptr) {
                                buffNewNode->Left = buffNode->Left;
                                buffNewNode->Right = new TNode(buffNode->Right->Height);
                                buffNode = buffNode->Right;
                                buffNewNode = buffNewNode->Right;
                            }
                            //at this point buffNode->Right->Right is nullptr
                            buffNewNode->Left = buffNode->Left;
                            //buffNewNode->Right is already nullptr
                            newRoot->Height = buffNode->Right->Height;
                            //newRoot->left is already set
                            newRoot->Right = Root->Right;
                            flag = false;
                        }
                    }
                    
                    if (buffNode->Right == nullptr) {
                            curNewNode->Height = buffNode->Height;
                            curNewNode->Left = buffNode->Left;
                            curNewNode->Right = nullptr;
                            delete buffNewNode;
                            flag = false;
                    } else {
                        while (buffNode->Right->Right != nullptr) {
                            buffNewNode->Left = buffNode->Left;
                            buffNewNode->Right = new TNode(buffNode->Right->Height);
                            buffNode = buffNode->Right;
                            buffNewNode = buffNewNode->Right;
                        }
                        //at this point buffNode->Right->Right is nullptr
                        buffNewNode->Left = buffNode->Left;
                        curNewNode->Height = buffNode->Right->Height;
                        curNewNode->Right = curNode->Right;
                        flag = false;
                    }
                }
            }
        }
    }
    Roots.push_back(newRoot);
    return newRoot;
}

#endif