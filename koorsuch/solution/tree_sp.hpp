#ifndef TREE_HPP
#define TREE_HPP


#include <iostream>
#include <vector>
#include <string>
#include <memory>

struct TNode {
    std::shared_ptr<TNode> Left;
    std::shared_ptr<TNode> Right;
    double Height;
    TNode(double h): Height(h) {}
    ~TNode() {}
};

class TTree {
public:
    std::vector<std::shared_ptr<TNode>> Roots;
    TTree() {}
    ~TTree() {}

    void PrintTree(int);
    std::shared_ptr<TNode> AddNode(double);
    std::shared_ptr<TNode> RemNode(double);
    bool Check(double);
    bool Check(double, TNode*);
    void FullCheck();
    void FullCheck(TNode*);
    std::vector<double> Values;

private:
    bool RecFind(TNode*, double &);
    void RecPrintTree(TNode*, int);
};

void VecRem(double h, std::vector<double> & vec) {
    unsigned int i = 0;
    while (i < vec.size() && vec.at(i) != h) {
        ++i;
    }
    if (i == vec.size()) {
        throw std::logic_error("");
    } else
    if (i == vec.size() - 1) {
        vec.pop_back();
        return;
    }
    for (; i < vec.size() - 1; ++i) {
        vec[i] = vec[i + 1];
    }
    vec.pop_back();
}

void TTree::FullCheck() {
    int cnt = 0;
    for (auto val : Values) {
        if (!Check(val)) {
            ++cnt;
        }
    }
    if (cnt > 0) {
        throw std::logic_error("Number of missing heights: " + std::to_string(cnt));
    }
}

void TTree::FullCheck(TNode* root) {
    int cnt = 0;
    for (auto val : Values) {
        if (!Check(val, root)) {
            ++cnt;
        }
    }
    if (cnt > 0) {
        throw std::logic_error("Number of missing heights: " + std::to_string(cnt));
    }
}

bool TTree::RecFind(TNode* node, double & h) {
    if (node == nullptr) {
        return false;
    }
    bool ret = false;
    if (h > node->Height) {
        ret = RecFind(node->Right.get(), h);
    } else
    if (h < node->Height) {
        ret = RecFind(node->Left.get(), h);
    } else {
        ret = true;
    }
    return ret;
}

bool TTree::Check(double h) {
    return RecFind(Roots.back().get(), h);
}

bool TTree::Check(double h, TNode* node) {
    return RecFind(node, h);
}

void TTree::RecPrintTree(TNode* node, int tab) {
    if (node->Right != nullptr) {
        RecPrintTree(node->Right.get(), tab + 1);
    }
    for (int i = 0; i < tab; ++i)
        std::cout << "  ";
    std::cout << node->Height << std::endl;
    if (node->Left != nullptr) {
        RecPrintTree(node->Left.get(), tab + 1);
    }
}

void TTree::PrintTree(int rootNum) {
    RecPrintTree(Roots.at(rootNum).get(), 0);
}

std::shared_ptr<TNode> TTree::AddNode(double h) {
    std::shared_ptr<TNode> Root;
    std::shared_ptr<TNode> newRoot;
    if (Roots.empty() || Roots.back() == nullptr) {
        newRoot = std::make_shared<TNode>(h);
        Roots.push_back(newRoot);

        Values.push_back(h);

        // FullCheck();

        return newRoot;
    } else {
        Root = Roots.back();
        newRoot = std::make_shared<TNode>(Root->Height);
    }
    std::shared_ptr<TNode> curNewNode = newRoot;
    std::shared_ptr<TNode> curNode = Root;
    bool flag = true;

    while (flag) {
        if (h < curNode->Height) {
            curNewNode->Right = curNode->Right;
            if (curNode->Left != nullptr) {
                curNewNode->Left = std::make_shared<TNode>(curNode->Left->Height);
                curNewNode = curNewNode->Left;
                curNode = curNode->Left;
            } else {
                curNewNode->Left = std::make_shared<TNode>(h);
                flag = false;
            }
        } else {
            curNewNode->Left = curNode->Left;
            if (curNode->Right != nullptr) {
                curNewNode->Right = std::make_shared<TNode>(curNode->Right->Height);
                curNewNode = curNewNode->Right;
                curNode = curNode->Right;
            } else {
                curNewNode->Right = std::make_shared<TNode>(h);
                flag = false;
            }
        }
    }

    Values.push_back(h);

    Roots.push_back(newRoot);

    // FullCheck();

    return newRoot;
}

std::shared_ptr<TNode> TTree::RemNode(double h) {
    std::shared_ptr<TNode> Root;
    if (Roots.empty()) {
        throw std::logic_error("Attempting to delete from empty tree");
    }
    Root = Roots.back();
    // if (!Check(h)) {
    //     std::cout << "GEGE in RemNode\n";
    // }
    std::shared_ptr<TNode> newRoot = std::make_shared<TNode>(Root->Height);
    std::shared_ptr<TNode> curNewNode = newRoot;
    std::shared_ptr<TNode> curNode = Root;
    std::shared_ptr<TNode> parent = newRoot;
    bool flag = true;

    VecRem(h, Values);

    while (flag) {
        if (h < curNode->Height) {
            curNewNode->Right = curNode->Right;
            if (curNode->Left != nullptr) {
                curNewNode->Left = std::make_shared<TNode>(curNode->Left->Height);
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
                curNewNode->Right = std::make_shared<TNode>(curNode->Right->Height);
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
                        newRoot = nullptr;
                    } else {
                        if (curNewNode == parent->Left) {
                            parent->Left = nullptr;
                        } else {
                            parent->Right = nullptr;
                        }
                    }
                    flag = false;
                    // FullCheck(newRoot);
                } else {    //there is right child
                    if (curNode == Root) {
                        newRoot = curNode->Right;
                    } else {
                        if (curNewNode == parent->Left) {
                            parent->Left = curNode->Right;
                        } else {
                            parent->Right = curNode->Right;
                        }
                    }
                    flag = false;
                    // FullCheck(newRoot);
                }
            } else {
                if (curNode->Right == nullptr) {//there is only left child
                    if (curNode == Root) {
                        newRoot = curNode->Left;
                    } else {
                        if (curNewNode == parent->Left) {
                            parent->Left = curNode->Left;
                        } else {
                            parent->Right = curNode->Left;
                        }
                    }
                    flag = false;
                    // FullCheck(newRoot);
                } else {    //there are both children
                    //go to the right
                    //walk through the left subtree copying all the nodes
                    //when the leaf is reached, swap cur and leaf
                    auto buffNode = curNode->Left;
                    auto buffNewNode = std::make_shared<TNode>(curNode->Left->Height);
                    curNewNode->Left = buffNewNode;

                    if (curNode == Root) {
                        if (buffNode->Right == nullptr) {
                            newRoot->Height = curNode->Left->Height;
                            newRoot->Right = Root->Right;
                            newRoot->Left = curNode->Left->Left;
                            flag = false;
                            // FullCheck(newRoot);
                        } else {
                            newRoot->Left = buffNewNode;
                            while (buffNode->Right->Right != nullptr) {
                                buffNewNode->Left = buffNode->Left;
                                buffNewNode->Right = std::make_shared<TNode>(buffNode->Right->Height);
                                buffNode = buffNode->Right;
                                buffNewNode = buffNewNode->Right;
                            }
                            //at this point buffNode->Right->Right is nullptr
                            buffNewNode->Left = buffNode->Left;
                            buffNewNode->Right = buffNode->Right;
                            newRoot->Height = buffNode->Right->Height;
                            //newRoot->left is already set
                            newRoot->Right = Root->Right;
                            flag = false;
                            // FullCheck(newRoot);
                        }
                    } else 
                    if (buffNode->Right == nullptr) {
                        curNewNode->Height = buffNode->Height;
                        curNewNode->Left = buffNode->Left;
                        curNewNode->Right = curNode->Right;
                        flag = false;
                        // FullCheck(newRoot);
                    } else {
                        while (buffNode->Right->Right != nullptr) {
                            buffNewNode->Left = buffNode->Left;
                            buffNewNode->Right = std::make_shared<TNode>(buffNode->Right->Height);
                            buffNode = buffNode->Right;
                            buffNewNode = buffNewNode->Right;
                        }
                        //at this point buffNode->Right->Right is nullptr
                        buffNewNode->Left = buffNode->Left;
                        buffNewNode->Right = buffNode->Right;
                        curNewNode->Height = buffNode->Right->Height;
                        curNewNode->Right = curNode->Right;
                        flag = false;
                        // FullCheck(newRoot);
                    }
                }
            }
        }
    }

    Roots.push_back(newRoot);

    // FullCheck();

    return newRoot;
}

#endif