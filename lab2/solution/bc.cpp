#include <iostream>
#include <map>
#include <cstring>
#include "tstring.hpp"
#include "tvector.hpp"

///check-existance func
///run after every move

const unsigned long long FILE_KEY = 4123978234870187237;

template <class T>
void swap(T & a, T & b) {
    T c;
    c = a;
    a = b;
    b = c;
}

class TBTree {
private:
    struct TNode {
        bool isLeaf;
        int keyCount;
        TVector<TString> keys;
        TVector<unsigned long long> data;
        TVector<TNode*> children;
        TNode* parent;
    };

    TNode* root;
    bool isSuchWord;
    bool doesAlreadyExist;
    int t;
    int nodeCount;

    template <class T>
    bool deleteFromArray(T* array, int size, int pos) {
        for (int i = pos + 1; i < size; ++i) {
            array[i - 1] = array[i];
        }
        return true;
    }

    unsigned long long recSearch(TNode* node, TString& key) {
        int i = 0;
        if (node->isLeaf) {
            for (i = 0; i < node->keyCount; ++i) {
                if (node->keys.at(i) == key) {
                    return (node->data.at(i));
                }
            }
        } else {
            for (i = 0; i < node->keyCount; ++i) {
                if (node->keys.at(i) == key) {
                    return (node->data.at(i));
                } else
                if (node->keys.at(i) > key) {
                    return recSearch(node->children.at(i), key);
                }
            }

            if (node->keys.at(i - 1) < key) {
                return recSearch(node->children.at(i), key);
            }
        }

        isSuchWord = false;
        return 0;
    }

    void nodeAddKey(TNode* node, TString& key, unsigned long long& data) {
        int i = 0;
        while (i < node->keyCount && node->keys.at(i) < key) {
            ++i;
        }
        node->keyCount += 1;
        node->keys.insert(key, i);
        node->data.insert(data, i);
        recCheckSize(root);
    }

    void recCheckParentage(TNode* node) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recCheckParentage(node->children.at(i));
                if (node->children.at(i)->parent != node) {
                    std::cout << "PARENTAGE ERROR: node " << node->children.at(i)->keys.at(0) << " counts " << node->children.at(i)->parent->keys.at(0) << " as his parent\n";
                }
            }
        }
    }

    int recCheckCounter;

    void _rCS(TNode* node) {
        if (node == nullptr) {throw(std::runtime_error("got nullptr\n"));}
        if (node->keyCount != node->keys.getSize()) {throw(std::runtime_error("keys size error\n"));}
        if (node->keyCount != node->data.getSize()) {throw(std::runtime_error("data size error\n"));}
        if (!node->isLeaf) {
            if (node->keyCount + 1 != node->children.getSize()) {
                std::cout << "children size = " << node->children.getSize() << ", node->keyCount = " << node->keyCount << std::endl;
                throw(std::runtime_error("children size error\n"));
            }
            for (int i = 0; i <= node->keyCount; ++i) {
                _rCS(node->children.at(i));
            }
        }
        recCheckCounter++;
    }

    void recCheckSize(TNode* node) {
        recCheckCounter = 0;
        _rCS(node);
        if (recCheckCounter != nodeCount) {
            std::cout << "recCheckCounter = " << recCheckCounter << ", nodeCount = " << nodeCount << std::endl;
            throw(std::runtime_error("nodeCount error\n"));
        }
    }

    TNode* nodeSplit(TNode* node, TNode* child) {
        if (node->keyCount < (2 * t - 1)) {
            throw(std::runtime_error("node is not full\n"));
        }
        if (node != root) {
            if (node->parent->keyCount == (2 * t - 1)) {
                node->parent = nodeSplit(node->parent, node);
            }
        }
        nodeCount++;
        TNode* newNode = new TNode;
        newNode->isLeaf = node->isLeaf;
        newNode->keyCount = t - 1;
        int i;
        if (node->isLeaf) {
            for (i = t; i < 2 * t - 1; ++i) {
                newNode->keys.insert(node->keys.at(i), i - t);
                newNode->data.insert(node->data.at(i), i - t);
            }
        } else {
            for (i = t; i < 2 * t - 1; ++i) {
                newNode->keys.insert(node->keys.at(i), i - t);
                newNode->data.insert(node->data.at(i), i - t);
                newNode->children.insert(node->children.at(i), i - t);
                newNode->children.at(i - t)->parent = newNode;
            }
            newNode->children.insert(node->children.at(i), i - t);
            newNode->children.at(i - t)->parent = newNode;
        }

        if (node != root) {
            //std::cout << "node is not root\n";
            newNode->parent = node->parent;
            node->parent->keyCount += 1;
            i = 0;
            while ((i < node->parent->keyCount - 2) && (node->parent->keys.at(i) < node->keys.at(t - 1))) {
                ++i;
            }
            if (node->parent->keys.at(i) > node->keys.at(t - 1)) {
                node->parent->keys.insert(node->keys.at(t - 1), i);
                node->parent->data.insert(node->data.at(t - 1), i);
                node->parent->children.insert(newNode, i + 1);
            } else {
                node->parent->keys.insert(node->keys.at(t - 1), i + 1);
                node->parent->data.insert(node->data.at(t - 1), i + 1);
                node->parent->children.insert(newNode, i + 2);
            }
        } else {    //if node == root
            //std::cout << "node is root\n";
            nodeCount += 1;
            TNode* newRoot = new TNode;
            node->parent = newRoot;
            newNode->parent = newRoot;

            newRoot->isLeaf = false;
            newRoot->keyCount = 1;
            newRoot->keys.insert(node->keys.at(t - 1), 0);
            newRoot->data.insert(node->data.at(t - 1), 0);
            newRoot->children.insert(node, 0);
            newRoot->children.insert(newNode, 1);
            root = newRoot;
        }

        node->keyCount = t - 1; //was 2t - 1
        node->keys.erase(t - 1, 2 * t - 2);
        node->data.erase(t - 1, 2 * t - 2);
        if (!node->isLeaf) {
            node->children.erase(t, 2 * t - 1);
        }
        if (!node->isLeaf) {
            for (i = 0; i <= node->keyCount; ++i) {
                if (node->children.at(i) == child) {
                    recCheckSize(root);
                    return node;
                } else
                if (newNode->children.at(i) == child) {
                    recCheckSize(root);
                    return newNode;
                }
            }
        }
        recCheckSize(root);
        return nullptr;
    }

    void recInsert(TNode* node, TString& key, unsigned long long& data) {
        int i = 0;
        if (!node->isLeaf) {
            for (i = 0; i < node->keyCount; ++i) {
                if (node->keys.at(i) == key) {
                    doesAlreadyExist = true;
                    return;
                } else
                if (node->keys.at(i) > key) {
                    recInsert(node->children.at(i), key, data);
                    return;
                }
            }
            if (node->keys.at(i - 1) < key) {
                recInsert(node->children.at(i), key, data);
                return;
            }
        } else {
            for (int i = 0; i < node->keyCount; ++i) {
                if (node->keys.at(i) == key) {
                    doesAlreadyExist = true;
                    return;
                }
            }
            if (node->keyCount < (2 * t - 1)) {
                nodeAddKey(node, key, data);
            } else {
                nodeSplit(node, nullptr);
                recInsert(node->parent, key, data);
            }
        }
        recCheckSize(root);
    }

    void recFreeTree(TNode* node) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recFreeTree(node->children.at(i));
            }
        }
        delete node;
    }

    void recPrintTree(TNode* node, int tab) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recPrintTree(node->children.at(i), tab + 1);
            }
        }
        for (int i = 0; i < tab; ++i) {
            std::cout << "  ";
        }
        for (int i = 0; i < node->keyCount; ++i) {
            std::cout << node->keys.at(i) << ' ';
        }
        std::cout << std::endl;
    }

    void recSave(TNode* node, FILE* out) {
        fwrite(&(node->isLeaf), sizeof(bool), 1, out);
        fwrite(&(node->keyCount), sizeof(int), 1, out);
        for (int i = 0; i < node->keyCount; ++i) {
            TString keyBuff = node->keys.at(i);
            fwrite(&keyBuff, sizeof(TString), 1, out);
        }
        for (int i = 0; i < node->keyCount; ++i) {
            unsigned long long dataBuff = node->data.at(i);
            fwrite(&dataBuff, sizeof(unsigned long long), 1, out);
        }
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recSave(node->children.at(i), out);
            }
        }
        return;
    }

    bool loadNode(TNode* node, TNode* parent, FILE* in) {
        fread(&(node->isLeaf), sizeof(bool), 1, in);
        if (ferror(in)) {
            return false;
        }

        fread(&(node->keyCount), sizeof(int), 1, in);
        if (ferror(in)) {
            return false;
        }

        TString keyBuff;
        for (int i = 0; i < node->keyCount; ++i) {
            fread(&keyBuff, sizeof(TString), 1, in);
            node->keys.insert(keyBuff, i);
        }

        if (ferror(in)) {
            return false;
        }

        unsigned long long dataBuff;
        for (int i = 0; i < node->keyCount; ++i) {
            fread(&dataBuff, sizeof(unsigned long long), 1, in);
            node->data.insert(dataBuff, i);
        }

        if (ferror(in)) {
            return false;
        }

        node->parent = parent;
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                TNode* newNode = new TNode;
                if (!(loadNode(newNode, node, in))) {
                    return false;
                }
                node->children.insert(newNode, i);
            }
        }
        return true;
    }

    bool deleteKey(TNode* node, const TString& key) {
        int i = 0;
        while (i < node->keyCount && node->keys.at(i) < key) {
            ++i;
        }
        if (node->keys.at(i) == key) {
            node->keys.erase(i);
            node->data.erase(i);
            node->keyCount -= 1;
            return true;
        } else {
            return false;
        }
    }

    TNode* lookForBros(TNode* node) {
        if (node == root)
            return nullptr;
        int i;
        for (i = 0; i <= node->parent->keyCount; ++i) {
            if (node->parent->children.at(i) == node) break;
        }
        if (i != 0 && node->parent->children.at(i - 1)->keyCount > t - 1) {
            return node->parent->children.at(i - 1);
        } else
        if (i != node->parent->keyCount && node->parent->children.at(i + 1)->keyCount > t - 1) {
            return node->parent->children.at(i + 1);
        } else {
            return nullptr;
        }
    }

    /* returns:
    0 if fill successful
    1 if node is root
    2 if no suitable brothers
    */
    int nodeFill(TNode* node) {
        if (node == root) {
            return 1;
        }
        int i;
        for (i = 0; i <= node->parent->keyCount; ++i) {
            if (node->parent->children.at(i) == node) break;
        }
        int flag = 0; // 0 -- no suitable bros, 1 -- lil bro, 2 -- big bro
        if (i == 0) {
            if (node->parent->children.at(i + 1)->keyCount > t - 1) {
                flag = 2;
            } else {
                flag = 0;
            }
        } else
        if (i == node->parent->keyCount) {
            if (node->parent->children.at(i - 1)->keyCount > t - 1) {
                flag = 1;
            } else {
                flag = 0;
            }
        } else {
            if (node->parent->children.at(i - 1)->keyCount > t - 1) {
                flag = 1;
            } else
            if (node->parent->children.at(i + 1)->keyCount > t - 1) {
                flag = 2;
            } else {
                flag = 0;
            }
        }

        if (flag == 0) {return 2;}

        TNode* bro = (flag == 1) ? node->parent->children.at(i - 1) : node->parent->children.at(i + 1);

        if (flag == 1) {
            node->keyCount++;
            //bro->keys.at(bro->keyCount - 1)
            node->keys.insert(node->parent->keys.at(i - 1), 0);
            node->data.insert(node->parent->data.at(i - 1), 0);
            node->parent->keys.erase(i - 1);
            node->parent->data.erase(i - 1);
            node->parent->keys.insert(bro->keys.at(bro->keyCount - 1), i - 1);
            node->parent->data.insert(bro->data.at(bro->keyCount - 1), i - 1);
            if (!node->isLeaf) {
                node->children.insert(bro->children.at(bro->keyCount), 0);
                node->children.at(0)->parent = node;
            }
            bro->keyCount--;
            bro->keys.erase(bro->keyCount);
            bro->data.erase(bro->keyCount);
            if (!bro->isLeaf) {
                bro->children.erase(bro->keyCount + 1);
            }
        } else {
            node->keyCount++;
            // node->keys.insert(bro->keys.at(0), node->keyCount - 1);
            // node->data.insert(bro->data.at(0), node->keyCount - 1);

            node->keys.insert(node->parent->keys.at(i), node->keyCount - 1);
            node->data.insert(node->parent->data.at(i), node->keyCount - 1);
            node->parent->keys.erase(i);
            node->parent->data.erase(i);
            node->parent->keys.insert(bro->keys.at(0), i);
            node->parent->data.insert(bro->data.at(0), i);

            if (!node->isLeaf) {
                node->children.insert(bro->children.at(0), node->keyCount);
                node->children.at(node->keyCount)->parent = node;
            }
            bro->keyCount--;
            bro->keys.erase(0);
            bro->data.erase(0);
            if (!node->isLeaf) {
                bro->children.erase(0);
            }
        }
        recCheckSize(root);
        return 0;
    }

    TNode* nodeMerge(TNode* nodeOrig, TNode* bratelnikOrig) {
        TNode* node = nodeOrig;
        TNode* bratelnik = bratelnikOrig;

        //check if node and bratelnik are siblings
        if (node == nullptr) {throw(std::runtime_error("node is null\n"));}
        if (bratelnik == nullptr) {throw(std::runtime_error("brother is null\n"));}
        if (node->parent != bratelnik->parent) {throw(std::runtime_error("parentage error\n"));}
        if (node->parent->children.getSize() != node->parent->keyCount + 1) {
            throw(std::runtime_error("keyCount or parent->children size error\n"));
        }     //commentable
        if (node->keyCount != t - 1 || bratelnik->keyCount != t - 1) {throw(std::runtime_error("keyCount is not minimal\n"));}

        int i = 0;
        while (i <= node->parent->keyCount && node->parent->children.at(i) != node) {
            ++i;
        }

        if (i > node->parent->keyCount) {throw(std::runtime_error("node is not child of its parent\n"));}
        if (i == 0 && node->parent->children.at(i + 1) != bratelnik) {
            throw(std::runtime_error("node and bratelnik arent nearest siblings type 1\n"));
        }
        if (i == node->parent->keyCount && node->parent->children.at(i - 1) != bratelnik) {
            throw(std::runtime_error("node and bratelnik arent nearest siblings type 2\n"));
        }

        //if two last children of root are being merged
        if (node->parent == root && node->parent->keyCount == 1) {
            if (i != 0) {//node is always little sibling
                swap(node, bratelnik);
                --i;
            }
            node->keyCount += 1;
            node->keys.insert(node->parent->keys.at(0), node->keyCount - 1);
            node->data.insert(node->parent->data.at(0), node->keyCount - 1);
            for (int j = 0; j < t - 1; ++j) {
                node->keys.insert(bratelnik->keys.at(j), node->keyCount);
                node->data.insert(bratelnik->data.at(j), node->keyCount);
                node->keyCount++;
            }
            if (!node->isLeaf) {
                for (int j = 0; j < t; ++j) {
                    node->children.insert(bratelnik->children.at(j), j + t);
                }
            }
            if (node->keyCount != 2 * t - 1) {throw(std::runtime_error("keyCount doesn't match\n"));}   //commentable


            nodeCount -= 2;
            delete node->parent;
            delete bratelnik;

            root = node;

            recCheckSize(root);

            return node;
        }

        //now we have to check is parent filled enough to merge its children
        if (node->parent->keyCount == t - 1 && node->parent != root) {  //parent isnt filled enough
            //try to fill, if fail, merge with its sibling
            if (nodeFill(node->parent) != 0) {  //cant be filled
                if (node->parent->parent->keyCount + 1 != node->parent->parent->children.getSize()) {
                    throw(std::runtime_error("parent parent keyCount and size dont match\n"));
                }   //commentable
                int j = 0;
                while (j <= node->parent->parent->keyCount && node->parent->parent->children.at(j) != node->parent) {
                    ++j;
                }
                if (j > node->parent->parent->keyCount) {
                    throw(std::runtime_error("parent is not child of its parent\n"));
                }   //commentable
                if (j == node->parent->parent->keyCount) {
                    if(node->parent->parent->children.at(j - 1)->keyCount != t - 1) {
                        throw(std::runtime_error("uncle is filled enough to share with parent\n"));
                    }
                    node->parent = nodeMerge(node->parent, node->parent->parent->children.at(j - 1));
                } else {
                    if(node->parent->parent->children.at(j + 1)->keyCount != t - 1) {
                        throw(std::runtime_error("uncle is filled enough to share with parent\n"));
                    }
                    node->parent = nodeMerge(node->parent, node->parent->parent->children.at(j + 1));
                }



                if (node->parent->keyCount != 2 * t - 1) {
                    throw(std::runtime_error("parent merging failed\n"));
                }   //commentable
            } else {
                if (node->parent->keyCount < t - 1 && node->parent->keyCount > 2 * t - 1) {
                    throw(std::runtime_error("filling failed\n"));
                }
            }
        }

        while (i <= node->parent->keyCount && node->parent->children.at(i) != node) {
            ++i;
        }

        //now we can merge
        //making bratelnik always big bro
        if (i != 0 && node->parent->children.at(i - 1) == bratelnik) {
            swap(node, bratelnik);
            --i;
        }
        //now bratelnik is big bro
        node->keyCount += 1;
        // if (i >= node->parent->keyCount) {
        //     std::cout << "i = " << i << ", node->parent->keyCount = " << node->parent->keyCount << std::endl;
        // }
        node->keys.insert(node->parent->keys.at(i), node->keyCount - 1);
        node->data.insert(node->parent->data.at(i), node->keyCount - 1);
        node->parent->keyCount--;
        node->parent->keys.erase(i);
        node->parent->data.erase(i);
        if (bratelnik != node->parent->children.at(i + 1)) {
            std::cout << "bratelnik = " << bratelnik << ", node = " << node << ", node->parent->children.at(i) and i + 1 and i - 1 = " << node->parent->children.at(i) << ", " << node->parent->children.at(i + 1) << ", " << (i != 0 ? node->parent->children.at(i + 1) : nullptr) << std::endl;
            throw(std::runtime_error("deleting wrong\n"));
        }
        node->parent->children.erase(i + 1);
        for (int cnt = 0; cnt < t - 1; ++cnt) {
            node->keys.insert(bratelnik->keys.at(cnt), node->keyCount);
            node->data.insert(bratelnik->data.at(cnt), node->keyCount);
            node->keyCount++;
        }
        if (!node->isLeaf) {
            for (int cnt = 0; cnt <= t - 1; ++cnt) {
                node->children.insert(bratelnik->children.at(cnt), t + cnt);
                node->children.at(t + cnt)->parent = node;
            }
        }
        if (node->keyCount != 2 * t - 1) {throw(std::runtime_error("keyCount doesnt match\n"));}

        nodeCount -= 1;
        delete bratelnik;
        recCheckSize(root);

        return node;
    }

    TNode* findRight(TNode* node) {
        if (!node->isLeaf) {
            return findRight(node->children.at(node->keyCount));
        } else {
            return node;
        }
    }

    TNode* findLeft(TNode* node) {
        if (!node->isLeaf) {
            return findRight(node->children.at(0));
        } else {
            return node;
        }
    }

    bool recDelete(TNode* node, const TString& key) {
        if (node == nullptr) {
            throw(std::runtime_error("null pointer exception\n"));
        }
        int j = 0;
        while (j < node->keyCount && node->keys.at(j) < key) {
            ++j;
        }
        if (j == node->keyCount) {
            if (!node->isLeaf) {
                return recDelete(node->children.at(j), key);
            } else {
                return false;
            }
        } else
        if (node->keys.at(j) > key) {
            if (!node->isLeaf) {
                return recDelete(node->children.at(j), key);
            } else {
                return false;
            }
        }
        if (!(node->keys.at(j) == key)) {throw(std::runtime_error("key exception\n"));}    //commentable

        //std::cout << "NODE->KEYS.AT(J) = " << node->keys.at(j) << ", key = " << key << std::endl;

        if (node->isLeaf) {
            if (node->keyCount > (t - 1) || node == root) { //what a luck
                node->keys.erase(j);
                node->data.erase(j);
                node->keyCount--;
                recCheckSize(root);
            } else {    //leaf is not filled enough
                //looking for keys from brothers
                int i = 0;
                while (i <= node->parent->keyCount && node != node->parent->children.at(i)) {
                    ++i;
                }
                if (i > node->parent->keyCount) {throw(std::runtime_error("i > keyCount"));}  //commentable
                TNode* brother = nullptr;
                bool isLilBro = false;
                if (i == node->parent->keyCount) {  //lil bro
                    if (node->parent->children.at(i - 1)->keyCount > t - 1) {
                        isLilBro = true;
                        brother = node->parent->children.at(i - 1);
                    }
                } else {    //big bro
                    if (node->parent->children.at(i + 1)->keyCount > t - 1) {
                        isLilBro = false;
                        brother = node->parent->children.at(i + 1);
                    }
                }
                if (brother != nullptr) {
                    if (isLilBro) {
                        node->keyCount++;
                        node->keys.insert(node->parent->keys.at(i - 1), 0);
                        node->data.insert(node->parent->data.at(i - 1), 0);
                        node->parent->data.erase(i - 1);
                        node->parent->keys.erase(i - 1);
                        node->parent->data.insert(brother->data.at(brother->keyCount - 1), i - 1);
                        node->parent->keys.insert(brother->keys.at(brother->keyCount - 1), i - 1);
                        brother->keyCount--;
                        brother->keys.erase(brother->keyCount);
                        brother->data.erase(brother->keyCount);
                    } else {
                        node->keyCount++;
                        node->keys.insert(node->parent->keys.at(i), 0);
                        node->data.insert(node->parent->data.at(i), 0);
                        node->parent->data.erase(i);
                        node->parent->keys.erase(i);
                        node->parent->data.insert(brother->data.at(0), i);
                        node->parent->keys.insert(brother->keys.at(0), i);
                        brother->keyCount--;
                        brother->keys.erase(0);
                        brother->data.erase(0);
                    }
                } else {    //no brothers suit
                    if (i == node->parent->keyCount) {  //lil bro
                        brother = node->parent->children.at(i - 1);
                    } else {    //big bro
                        brother = node->parent->children.at(i + 1);
                    }
                    TNode* nodeBuff = nodeMerge(node, brother);
                    recDelete(nodeBuff, key);
                }
            }
        } else {    //its not leaf monkaS
            //trying left subtree
            TString keyBuff;
            unsigned long long dataBuff;
            if (node->children.at(j)->keyCount > t - 1) {
                TNode* leftSubTree = findRight(node->children.at(j));
                keyBuff = leftSubTree->keys.at(leftSubTree->keyCount - 1);
                dataBuff = leftSubTree->data.at(leftSubTree->keyCount - 1);
                recDelete(leftSubTree, keyBuff);
                node->keys.erase(j);
                node->data.erase(j);
                node->keys.insert(keyBuff, j);
                node->data.insert(dataBuff, j);
            } else
            if (node->children.at(j + 1)->keyCount > t - 1) {
                TNode* rightSubTree = findLeft(node->children.at(j + 1));
                keyBuff = rightSubTree->keys.at(0);
                dataBuff = rightSubTree->data.at(0);
                recDelete(rightSubTree, keyBuff);
                node->keys.erase(j);
                node->data.erase(j);
                node->keys.insert(keyBuff, j);
                node->data.insert(dataBuff, j);
            } else {
                //std::cout << "Node keyCount = " << node->keyCount << "node children count = " << node->children.getSize() << std::endl;
                TNode* nodeBuff = nodeMerge(node->children.at(j), node->children.at(j + 1));
                recDelete(nodeBuff, key);
            }
        }
        recCheckSize(root);
        return true;
    }

public:
    void init(int treeMeasure) {
        root = new TNode;
        root->keyCount = 0;
        root->isLeaf = true;
        root->parent = nullptr;
        t = treeMeasure;
        nodeCount = 1;
    }

    TBTree() {
        init(0);
    }

    TBTree(int treeMeasure) {
        init(treeMeasure);
    }

    ~TBTree() {
        recFreeTree(root);
    }

    void search(TString& key) {
        isSuchWord = true;
        unsigned long long result = recSearch(root, key);
        // if (isSuchWord) {
        //     std::cout << "OK: " << result << std::endl;
        // } else {
        //     std::cout << "NoSuchWord\n";
        // }
        return;
    }

    bool insert(TString& key, unsigned long long& data) {
        doesAlreadyExist = false;
        recInsert(root, key, data);
        return !doesAlreadyExist;
    }

    void printTree() {
        recPrintTree(root, 0);
    }

    void save(FILE* out) {
        fwrite(&FILE_KEY, sizeof(unsigned long long), 1, out);
        fwrite(&t, sizeof(int), 1, out);    //save t
        recSave(root, out);     //recSave(root)
    }

    bool load(FILE* in) {
        unsigned long long buff;
        fread(&buff, sizeof(unsigned long long), 1, in);
        if (ferror(in) != 0) {
            return false;
        }
        if (buff != FILE_KEY) {
            return false;
        }
        fread(&t, sizeof(int), 1, in);
        if (ferror(in)) {
            return false;
        }
        recFreeTree(root);
        init(t);
        return loadNode(root, nullptr, in);
    }

    int getMeasure() {
        return t;
    }

    bool deleteKeyFromTree(TString& key) {
        if (root->keyCount == 0) {return false;}
        return recDelete(root, key);
    }

    void checkParentage() {
        recCheckParentage(root);
    }

};


int main() {
    long double stdMapTime = 0;
    long double bTreeTime = 0;
    long double start, end;
    std::map<std::string, unsigned long long> mp;

    TBTree* tree = new TBTree(2);
    TString curKey;
    char q[256];
    unsigned short curSize = 0;
    unsigned long long curValue;
    char nullStr = '\0';

    while (scanf("%s", q) > 0) {
        if (q[0] == '+') {
            scanf("%s%llu", q, &curValue);
            toLowerCase(q);
            stringAssignment(curKey.string, q);

            //BTree
            start = clock();
            tree->insert(curKey, curValue);
            end = clock();
            bTreeTime = bTreeTime + end - start;

            //std::map
            start = clock();
            if (mp.find(curKey.string) == mp.end()) {
                mp[curKey.string] = curValue; 
            }
            end = clock();
            stdMapTime = stdMapTime + end - start;
        } else 
        if (q[0] == '-') {
            scanf("%s", q);
            toLowerCase(q);
            stringAssignment(curKey.string, q);

            //BTree
            start = clock();
            tree->deleteKeyFromTree(curKey);
            end = clock();
            bTreeTime = bTreeTime + end - start;

            //std::map
            start = clock();
            if (mp.find(curKey.string) == mp.end()) {
                mp.erase(curKey.string); 
            }
            end = clock();
            stdMapTime = stdMapTime + end - start;
        } else {
            toLowerCase(q);
            stringAssignment(curKey.string, q);

            //BTree
            start = clock();
            tree->search(curKey);
            end = clock();
            bTreeTime = bTreeTime + end - start;

            //std::map
            start = clock();
			mp.find(curKey.string);
			end = clock();
			stdMapTime = stdMapTime + end - start;
        }
        stringAssignment(curKey.string, &nullStr);
    }
    delete tree;
    printf("B-tree: %.3Lf ms\n", bTreeTime / 1000.0);
	printf("std::map: %.3Lf ms\n", stdMapTime / 1000.0);
	return 0;
}