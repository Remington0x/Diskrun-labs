#include <iostream>
#include "tstring.hpp"

const unsigned long long FILE_KEY = 4123978234870187237;

class TBTree {
private:
    struct TNode {
        bool isLeaf;
        int keyCount;
        TString* keys;
        unsigned long long* data;
        TNode** children;
        TNode* parent;
    };

    TNode* root;
    bool isSuchWord;
    bool doesAlreadyExist;
    int t;

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
                if (node->keys[i] == key) {
                    return (node->data[i]);
                }
            }
        } else {
            for (i = 0; i < node->keyCount; ++i) {
                if (node->keys[i] == key) {
                    return (node->data[i]);
                } else
                if (node->keys[i] > key) {
                    return recSearch(node->children[i], key);
                }
            }

            if (node->keys[i - 1] < key) {
                return recSearch(node->children[i], key);
            }
        }

        isSuchWord = false;
        return 0;
    }

    void nodeAddKey(TNode* node, TString& key, unsigned long long& data) {
        int i = 0;
        while (i < node->keyCount && node->keys[i] < key) {
            ++i;
        }

        node->keyCount += 1;
        node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
        node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);

        insertInArray(node->keys, node->keyCount, key, i);
        insertInArray(node->data, node->keyCount, data, i);
    }

    void nodeSplit(TNode* node) {
        if (node->keyCount < (2 * t - 1)) {
            std::cout << "ERROR: nodeSplit(): node is not full\n";
            return;
        }
        //if node is not root

        TNode* newNode = (TNode*)malloc(sizeof(TNode));
        newNode->isLeaf = node->isLeaf;

        newNode->keyCount = t - 1;
        newNode->keys = (TString*)malloc(sizeof(TString) * newNode->keyCount);
        newNode->data = (unsigned long long*)malloc(sizeof(unsigned long long) * newNode->keyCount);
        int i;
        if (node->isLeaf) {
            for (i = t; i < 2 * t - 1; ++i) {
                newNode->keys[i - t] = node->keys[i];
                newNode->data[i - t] = node->data[i];
            }
        } else {
            newNode->children = (TNode**)malloc(sizeof(TNode*) * (newNode->keyCount + 1));
            for (i = t; i < 2 * t - 1; ++i) {
                newNode->keys[i - t] = node->keys[i];
                newNode->data[i - t] = node->data[i];
                newNode->children[i - t] = node->children[i];
                newNode->children[i - t]->parent = newNode;
            }
            newNode->children[i - t] = node->children[i];
            newNode->children[i - t]->parent = newNode;
        }

        if (node != root) {
            newNode->parent = node->parent;
            //parent node manipulations
            if (node->parent->keyCount == (2 * t - 1)) { //after these words there started a pure horror in the ukranian train
                nodeSplit(node->parent);
            }
            node->parent->keyCount += 1;
            node->parent->keys = (TString*)realloc(node->parent->keys, sizeof(TString) * node->parent->keyCount);
            node->parent->data = (unsigned long long*)realloc(node->parent->data, sizeof(unsigned long long) * node->parent->keyCount);
            node->parent->children = (TNode**)realloc(node->parent->children, sizeof(TNode*) * (node->parent->keyCount + 1));
            i = 0;
            while ((i < node->parent->keyCount - 1) && (node->parent->keys[i] < node->keys[t - 1])) {
                ++i;
            }
            insertInArray(node->parent->keys, node->parent->keyCount, node->keys[t - 1], i);
            insertInArray(node->parent->data, node->parent->keyCount, node->data[t - 1], i);
            insertInArray(node->parent->children, node->parent->keyCount + 1, newNode, i + 1);

        } else {    //if node == root
            TNode* newRoot = (TNode*)malloc(sizeof(TNode));
            node->parent = newRoot;
            newNode->parent = newRoot;

            newRoot->isLeaf = false;
            newRoot->keyCount = 1;
            newRoot->keys = (TString*)malloc(sizeof(TString) * newRoot->keyCount);
            newRoot->keys[0] = node->keys[t - 1];
            newRoot->data = (unsigned long long*)malloc(sizeof(unsigned long long) * newRoot->keyCount);
            newRoot->data[0] = node->data[t - 1];
            newRoot->children = (TNode**)malloc(sizeof(TNode*) * (newRoot->keyCount + 1));
            newRoot->children[0] = node;
            newRoot->children[1] = newNode;
            root = newRoot;
        }

        //shrink node
        node->keyCount = t - 1;
        node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
        node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
        node->children = (TNode**)realloc(node->children, sizeof(TNode*) * (node->keyCount + 1));

    }

    template <class T>
    void insertInArray(T* array, int size, T instance, int pos) { //pos 0..i-1
        for (int i = size - 2; i >= pos; --i) {
            array[i + 1] = array[i];
        }
        array[pos] = instance;
    }

    void recInsert(TNode* node, TString& key, unsigned long long& data) {
        int i = 0;
        if (!node->isLeaf) {
            for (i = 0; i < node->keyCount; ++i) {
                if (node->keys[i] == key) {
                    doesAlreadyExist = true;
                    return;
                } else
                if (node->keys[i] > key) {
                    recInsert(node->children[i], key, data);
                    return;
                }
            }
            if (node->keys[i - 1] < key) {
                recInsert(node->children[i], key, data);
                return;
            }
        } else {    //seems like we have to insert into this leaf
            for (int i = 0; i < node->keyCount; ++i) {
                if (node->keys[i] == key) {
                    doesAlreadyExist = true;
                    return;
                }
            }
            if (node->keyCount < (2 * t - 1)) { //leaf is not full
                nodeAddKey(node, key, data);
            } else {    //time to split
                nodeSplit(node);    //split
                recInsert(node->parent, key, data);    //add
            }
        }
    }

    void recFreeTree(TNode* node) {
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recFreeTree(node->children[i]);
            }
            free(node->children);
        }

        free(node->keys);
        free(node->data);
        free(node);
    }

    void recPrintTree(TNode* node, int tab) {
        // std::cout << "Come into key[0] = " << node->keys[0] << ", data[0] = " << node->data[0] << std::endl;
        // std::cout << "Printing...\n";
        if (!node->isLeaf) {
            // std::cout << "Is leaf = " << node->isLeaf << std::endl;
            // std::cout << "children detected, printing them...\n";
            for (int i = 0; i <= node->keyCount; ++i) {
                recPrintTree(node->children[i], tab + 1);
            }
        }
        for (int i = 0; i < tab; ++i) {
            std::cout << "  ";
        }
        for (int i = 0; i < node->keyCount; ++i) {
            std::cout << node->keys[i] << ' ';
        }
        std::cout << std::endl;
    }

    void recSave(TNode* node, FILE* out) {
        fwrite(&(node->isLeaf), sizeof(bool), 1, out);
        fwrite(&(node->keyCount), sizeof(int), 1, out);
        fwrite(node->keys, sizeof(TString), node->keyCount, out);
        fwrite(node->data, sizeof(unsigned long long), node->keyCount, out);
        if (!node->isLeaf) {
            for (int i = 0; i <= node->keyCount; ++i) {
                recSave(node->children[i], out);
            }
        }
        return;
    }

    bool loadNode(TNode* node, TNode* parent, FILE* in) {
        // std::cout << "loadNode in work...\n";
        fread(&(node->isLeaf), sizeof(bool), 1, in);
        if (ferror(in)) {
            return false;
        }
        // std::cout << "isLeaf = " << node->isLeaf << std::endl;
                        // return false;
        fread(&(node->keyCount), sizeof(int), 1, in);
        if (ferror(in)) {
            return false;
        }
        // std::cout << "keyCount = " << node->keyCount << std::endl;
        node->keys = (TString*)malloc(sizeof(TString) * node->keyCount);
        fread(node->keys, sizeof(TString), node->keyCount, in);
        if (ferror(in)) {
            return false;
        }
        node->data = (unsigned long long*)malloc(sizeof(unsigned long long) * node->keyCount);
        fread(node->data, sizeof(unsigned long long), node->keyCount, in);
        if (ferror(in)) {
            return false;
        }
        node->parent = parent;
        if (!node->isLeaf) {
            node->children = (TNode**)malloc(sizeof(TNode*) * (node->keyCount + 1));
            for (int i = 0; i <= node->keyCount; ++i) {
                node->children[i] = (TNode*)malloc(sizeof(TNode));
                if (!(loadNode(node->children[i], node, in))) {
                    return false;
                }
            }
        }
        return true;
    }

    bool deleteKey(TNode* node, TString& key) {
        int i = 0;
        while (node->keys[i] < key && i < node->keyCount) {
            ++i;
        }
        if (node->keys[i] == key) {
            if (!deleteFromArray(node->keys, node->keyCount, i)) {return false;}
            if (!deleteFromArray(node->data, node->keyCount, i)) {return false;}
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
            if (node->parent->children[i] == node) break;
        }
        if (i != 0 && node->parent->children[i - 1]->keyCount > t - 1) {
            return node->parent->children[i - 1];
        } else
        if (i != node->parent->keyCount && node->parent->children[i + 1]->keyCount > t - 1) {
            return node->parent->children[i + 1];
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
        //look for bros
        if (node == root) {
            return 1;
        }
        int i;
        for (i = 0; i <= node->parent->keyCount; ++i) {
            if (node->parent->children[i] == node) break;
        }
        int flag = 0; // 0 -- no suitable bros, 1 -- lil bro, 2 -- big bro
        if (i == 0) {
            if (node->parent->children[i + 1]->keyCount > t - 1) {
                flag = 2;
            } else {
                flag = 0;
            }
        } else
        if (i == node->parent->keyCount) {
            if (node->parent->children[i - 1]->keyCount > t - 1) {
                flag = 1;
            } else {
                flag = 0;
            }
        } else {
            if (node->parent->children[i - 1]->keyCount > t - 1) {
                flag = 1;
            } else
            if (node->parent->children[i + 1]->keyCount > t - 1) {
                flag = 2;
            } else {
                flag = 0;
            }
        }

        if (flag == 0) {return 2;}

        TNode* bro = (flag == 1) ? node->parent->children[i - 1] : node->parent->children[i + 1];

        if (flag == 1) {
            node->keyCount++;
            node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
            node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
            insertInArray(node->keys, node->keyCount, bro->keys[bro->keyCount - 1], 0);
            insertInArray(node->data, node->keyCount, bro->data[bro->keyCount - 1], 0);
            if (!node->isLeaf) {
                node->children = (TNode**)realloc(node->children, sizeof(TNode*) * (node->keyCount + 1));
                insertInArray(node->children, node->keyCount, bro->children[bro->keyCount], 0);
                node->children[0]->parent = node;
            }
            bro->keyCount--;
            bro->keys = (TString*)realloc(bro->keys, sizeof(TString) * bro->keyCount);
            bro->data = (unsigned long long*)realloc(bro->data, sizeof(unsigned long long) * bro->keyCount);
            if (!node->isLeaf) {
                bro->children = (TNode**)realloc(bro->children, sizeof(TNode*) * (bro->keyCount + 1));
            }
        } else {
            node->keyCount++;
            node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
            node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
            insertInArray(node->keys, node->keyCount, bro->keys[0], node->keyCount - 1);
            insertInArray(node->data, node->keyCount, bro->data[0], node->keyCount - 1);
            if (!node->isLeaf) {
                node->children = (TNode**)realloc(node->children, sizeof(TNode*) * (node->keyCount + 1));
                insertInArray(node->children, node->keyCount, bro->children[0], node->keyCount);
                node->children[node->keyCount]->parent = node;
            }
            deleteFromArray(bro->keys, bro->keyCount, 0);
            deleteFromArray(bro->data, bro->keyCount, 0);
            bro->keyCount--;
            bro->keys = (TString*)realloc(bro->keys, sizeof(TString) * bro->keyCount);
            bro->data = (unsigned long long*)realloc(bro->data, sizeof(unsigned long long) * bro->keyCount);
            if (!node->isLeaf) {
                deleteFromArray(bro->children, bro->keyCount + 1, 0);
                bro->children = (TNode**)realloc(bro->children, sizeof(TNode*) * (bro->keyCount + 1));
            }
        }
        return 0;
    }

    //this function is unsafe because of freeing one of node or bratelnik
    TNode* nodeMerge(TNode* node, TNode* bratelnik) {
        if (node->parent == root && node->parent->keyCount == 1) {
            std::cout << "node->parent == root and root->keyCount == 1\n";
            //node = left son
            node = node->parent->children[0];
            bratelnik = node->parent->children[1];
            node->keyCount = 2 * t - 1;
            node->keys = (TString*)realloc(node->keys, node->keyCount * sizeof(TString));
            node->data = (unsigned long long*)realloc(node->data, node->keyCount * sizeof(unsigned long long));
            if (!node->isLeaf) {
                node->children = (TNode**)realloc(node->children, sizeof(TNode*) * (node->keyCount + 1));
                for (int i = t; i <= node->keyCount; ++i) {
                    node->children[i] = bratelnik->children[i - t];
                }
            }
            node->keys[t - 1] = node->parent->keys[0];
            node->data[t - 1] = node->parent->data[0];
            for (int i = t; i < node->keyCount; ++i) {
                node->keys[i] = bratelnik->keys[i - t];
                node->data[i] = bratelnik->data[i - t];
            }
            free(node->parent->data);
            free(node->parent->keys);
            free(node->parent->children);
            free(node->parent);

            free(bratelnik->data);
            free(bratelnik->keys);
            free(bratelnik->children);
            free(bratelnik);

            root = node;

            return node;
        } else {
            std::cout << "node->parent != root or root->keyCount != 1\n";
            if (node->parent->keyCount == (t - 1) && node->parent != root) {  //parent isn filled enough
                std::cout << "parent isn filled enough\n";
                if (nodeFill(node->parent) != 0) {  //parent cant be filled with nodeFill()
                    std::cout << "parent cant be filled with nodeFill()\n";
                    int i = 0;
                    for (i = 0; node != node->parent->children[i]; ++i) {}
                    TNode* uncle = (i == 0) ? node->parent->children[i + 1] : node->parent->children[i - 1];
                    nodeMerge(node->parent, uncle);
                }
                std::cout << "parent is filled\n";
                //parent is filled
            }
            std::cout << "parent is filled ENOUGH ALREADY\n";
            TString keyBuff;
            unsigned long long dataBuff;
            int i = 0;
            while (node->parent->children[i] != node) {
                ++i;
            }
            std::cout << "node posititon in parent->children is " << i << std::endl;

            if (bratelnik == node->parent->children[i - 1]) {   //lil bro
                std::cout << "lil bro\n";
                keyBuff = node->parent->keys[i - 1];
                dataBuff = node->parent->data[i - 1];
                deleteFromArray(node->parent->children, node->parent->keyCount + 1, i);
                // for (int counter1 = i + 1; counter1 <= node->parent->keyCount; ++counter1) {
                //     node->parent->children[counter1 - 1] = node->parent->children[counter1];
                // }
                deleteKey(node->parent, node->parent->keys[i - 1]);
                node->parent->keys = (TString*)realloc(node->parent->keys, sizeof(TString) * node->parent->keyCount);
                node->parent->data = (unsigned long long*)realloc(node->parent->data, sizeof(unsigned long long) * node->parent->keyCount);
                node->parent->children = (TNode**)realloc(node->parent->children, sizeof(TNode*) * (node->parent->keyCount + 1));
                bratelnik->keyCount = 2 * t - 1;
                bratelnik->keys = (TString*)realloc(bratelnik->keys, sizeof(TString) * bratelnik->keyCount);
                bratelnik->data = (unsigned long long*)realloc(bratelnik->data, sizeof(unsigned long long) * bratelnik->keyCount);
                bratelnik->keys[t - 1] = keyBuff;
                bratelnik->data[t - 1] = dataBuff;
                for (int i = t; i < bratelnik->keyCount; ++i) {
                    bratelnik->keys[i] = node->keys[i - t];
                    bratelnik->data[i] = node->data[i - t];
                }
                if (!node->isLeaf) {
                    bratelnik->children = (TNode**)realloc(bratelnik->children, sizeof(TNode*) * (bratelnik->keyCount + 1));
                    for (int i = t; i <= bratelnik->keyCount; ++i) {
                        bratelnik->children[i] = node->children[i - t];
                        bratelnik->children[i]->parent = bratelnik;
                    }
                }
                free(node->children);
                free(node->data);
                free(node->keys);
                free(node);
                return bratelnik;
            } else {    //big bro
                std::cout << "big bro\n";

                keyBuff = node->parent->keys[i];
                dataBuff = node->parent->data[i];

                std::cout << "keyBuff = " << keyBuff << ", dataBuff = " << dataBuff << std::endl;

                std::cout << "node->parent->keyCount = " << node->parent->keyCount << ", node->parent->children[1]->keys[0] = " << node->parent->children[1]->keys[0] << std::endl;
                deleteFromArray(node->parent->children, node->parent->keyCount + 1, i + 1);
                std::cout << "node->parent->keyCount = " << node->parent->keyCount << ", node->parent->children[1]->keys[0] = " << node->parent->children[1]->keys[0] << std::endl;
                std::cout << "node->parent->keyCount = " << node->parent->keyCount << ", node->parent->children[2]->keys[0] = " << node->parent->children[2]->keys[0] << std::endl;
                deleteKey(node->parent, node->parent->keys[i]);
                node->parent->keys = (TString*)realloc(node->parent->keys, sizeof(TString) * node->parent->keyCount);
                node->parent->data = (unsigned long long*)realloc(node->parent->data, sizeof(unsigned long long) * node->parent->keyCount);
                node->parent->children = (TNode**)realloc(node->parent->children, sizeof(TNode*) * (node->parent->keyCount + 1));

                std::cout << "node->parent->keyCount = " << node->parent->keyCount << ", node->parent->children[1]->keys[0] = " << node->parent->children[1]->keys[0] << std::endl;

                node->keyCount = 2 * t - 1;
                node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
                node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
                node->keys[t - 1] = keyBuff;
                node->data[t - 1] = dataBuff;
                for (int i = t; i < node->keyCount; ++i) {
                    node->keys[i] = bratelnik->keys[i - t];
                    node->data[i] = bratelnik->data[i - t];
                }
                if (!node->isLeaf) {
                    node->children = (TNode**)realloc(node->children, sizeof(TNode*) * (node->keyCount + 1));
                    for (int i = t; i <= node->keyCount; ++i) {
                        node->children[i] = bratelnik->children[i - t];
                        node->children[i] = node;
                    }
                }
                free(bratelnik->children);
                free(bratelnik->data);
                free(bratelnik->keys);
                free(bratelnik);
                return node;
            }
        }
        return nullptr;

    }

    TNode* findRight(TNode* node) {
        if (!node->isLeaf) {
            return findRight(node->children[node->keyCount]);
        } else {
            return node;
        }
    }

    TNode* findLeft(TNode* node) {
        if (!node->isLeaf) {
            return findRight(node->children[0]);
        } else {
            return node;
        }
    }

    //this function is probably unsafe because of nodeMerge, TNode* node might be freed inside
    bool recDelete(TNode* node, TString& key) {
        if (node == nullptr) {
            return false;
        }
        int j = 0;
        while (node->keys[j] < key && j < node->keyCount) {
            std::cout << "NODE->keys[j] = " << node->keys[j] << ", key = " << key << std::endl;
            ++j;
        }
        //j = pos of key, j = pos of array member more than key, j = node->keyCount
        if (j == node->keyCount) {
            if (!node->isLeaf) {
                return recDelete(node->children[j], key);
            } else {
                std::cout << "j == node->keyCount\n";
                return false;
            }
        } else
        if (node->keys[j] > key) {
            return recDelete(node->children[j], key);
        }

        std::cout << "node->keys[0] = " << node->keys[0] << std::endl;
        std::cout << "Node identified successfully\n";
        //at this moment i think i already found neccesary node
        if (node->isLeaf) {
            std::cout << "node is leaf\n";
            if (node->keyCount > (t - 1) || node == root) { //what a luck
                std::cout << "node->keyCount = " << node->keyCount << std::endl;
                std::cout << "Leaf is filled enough\n";
                deleteKey(node, key);
                node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
                node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
            } else {    //leaf is not filled enough
                std::cout << "leaf is not filled enough\n";
                TNode* brother;
                brother = lookForBros(node);
                std::cout << "look for bros success\n";
                if (brother != nullptr) {
                    std::cout << "brother found\n";
                    TString k1, k2;
                    unsigned long long d1, d2;
                    int i = 0;

                    //pick k2 from parent
                    //pick divider key from brother (k1)
                    for (i = 0; i <= node->parent->keyCount; ++i) {
                        if (node->parent->children[i] == node) {break;}
                    }
                    bool isLilBro = brother->keys[0] < node->keys[0];
                    if (isLilBro) { //lil bro
                        k1 = brother->keys[brother->keyCount - 1];
                        d1 = brother->data[brother->keyCount - 1];
                        k2 = node->parent->keys[i - 1];
                        d2 = node->parent->data[i - 1];
                    } else {    //big bro
                        k1 = brother->keys[0];
                        d1 = brother->data[0];
                        k2 = node->parent->keys[i];
                        d2 = node->parent->data[i];
                    }
                    //i - node posititon in parent->children[]
                    //or

                    //delete key
                    if (!deleteKey(node, key)) {return false;}
                    //k2 to node, k1 to parent
                    if (isLilBro) {
                        node->keyCount += 1;
                        insertInArray(node->keys, node->keyCount, k2, 0);
                        insertInArray(node->data, node->keyCount, d2, 0);
                        node->parent->keys[i - 1] = k1;
                        node->parent->data[i - 1] = d1;
                    } else {
                        node->keyCount += 1;
                        insertInArray(node->keys, node->keyCount, k2, node->keyCount - 1);
                        insertInArray(node->data, node->keyCount, d2, node->keyCount - 1);
                        node->parent->keys[i] = k1;
                        node->parent->data[i] = d1;
                    }

                    if (!deleteKey(brother, k1)) {
                        return false;
                    } else {
                        brother->keys = (TString*)realloc(brother->keys, sizeof(TString) * brother->keyCount);
                        brother->data = (unsigned long long*)realloc(brother->data, sizeof(unsigned long long) * brother->keyCount);
                    }
                } else {    //there is no suitable bratelnik
                    std::cout << "brother not found\n";
                    bool isLilBro = !(node->parent->children[0] == node);
                    int i;
                    for (i = 0; i <= node->parent->keyCount; ++i) {
                        if (node->parent->children[i] == node) {break;}
                    }
                    brother = (isLilBro) ? node->parent->children[i - 1] : node->parent->children[i + 1];
                    node = nodeMerge(node, brother);

                    deleteKey(node, key);
                    node->keys = (TString*)realloc(node->keys, sizeof(TString) * node->keyCount);
                    node->data = (unsigned long long*)realloc(node->data, sizeof(unsigned long long) * node->keyCount);
                }
            }
        } else {    //its not leaf monkaS
            //son->keyCount > (t - 1) ?
            int i;
            for (i = 0; i <= node->keyCount; ++i) {
                if (node->keys[i] == key) {break;}
            }

            if (node->children[i]->keyCount > (t - 1)) {    //prev son suits
                std::cout << "prev son suits\n";
                TString k1;
                unsigned long long d1;
                //find ultraright key
                TNode* buff = findRight(node->children[i]);
                k1 = buff->keys[buff->keyCount - 1];
                d1 = buff->data[buff->keyCount - 1];
                recDelete(buff, k1);
                node->keys[i] = k1;
                node->data[i] = d1;
            } else
            if (node->children[i + 1]->keyCount > (t - 1)) {//next son suits
                std::cout << "next son suits\n";
                TString k1;
                unsigned long long d1;
                //find ultraleft key
                TNode* buff = findLeft(node->children[i + 1]);
                k1 = buff->keys[0];
                d1 = buff->data[0];
                recDelete(buff, k1);
                node->keys[i] = k1;
                node->data[i] = d1;
            } else {    //both sons arent filled enough
                std::cout << "both sons arent filled enough\n";
                std::cout << "MERGING " << node->children[i]->keys[0] << " AND " << node->children[i + 1]->keys[0] << std::endl;
                TNode* son = nodeMerge(node->children[i], node->children[i + 1]);
                TString counter;
                counter.string[1] = '\0';
                for (counter.string[0] = 'a'; counter.string[0] <= 'j'; ++counter.string[0]) {
                    search(counter);
                }
                recDelete(son, key);
            }
        }
        return true;
    }

public:
    void init(int treeMeasure) {
        root = (TNode*)malloc(sizeof(TNode));
        root->keyCount = 0;
        root->isLeaf = true;
        root->children = nullptr;
        root->data = nullptr;
        root->keys = nullptr;
        root->parent = nullptr;
        t = treeMeasure;
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
        if (isSuchWord) {
            std::cout << "OK: " << result << std::endl;
        } else {
            std::cout << "NoSuchWord\n";
        }
        return;
    }

    //returns true if insertion successful, false if word already exists
    bool insert(TString& key, unsigned long long& data) {
        doesAlreadyExist = false;
        recInsert(root, key, data);
        return !doesAlreadyExist;
    }

    void printTree() {
        //std::cout << root->keyCount << ' ' << root->keys[0] << ' ' << root->data[0] << std::endl;
        recPrintTree(root, 0);
    }

    void save(FILE* out) {
        fwrite(&FILE_KEY, sizeof(unsigned long long), 1, out);
        fwrite(&t, sizeof(int), 1, out);    //save t
        recSave(root, out);     //recSave(root)
    }

    bool load(FILE* in) {
        //check if file is corrupted
        unsigned long long buff;
        fread(&buff, sizeof(unsigned long long), 1, in);
        if (ferror(in) != 0) {
            // std::cout << "ferror\n";
            return false;
        }
        if (buff != FILE_KEY) {
            // std::cout << "buff != FILE_KEY\n";
            // std::cout << "buff = " << buff << ", FILE_KEY = " << FILE_KEY << std::endl;
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


};



int main() {
    // std::cout << "Enter a and b\n";
    // TString a, b;
    // std::cin >> a.string >> b.string;
    // std::cout << "a < b = " << (a < b) << std::endl;
    // std::cout << "a > b = " << (a > b) << std::endl;
    // std::cout << "a == b = " << (a == b) << std::endl;
    // std::cout << "a = b" << std::endl;
    // a = b;
    // std::cout << a << std::endl;

    // std::cout << "sizeof TString = " << sizeof(TString) << std::endl;
    // std::cout << "sizeof unsigned long long = " << sizeof(unsigned long long) << std::endl;

    //bool flag = true;
    TBTree* tree = new TBTree(2);
    TBTree* buffTree = new TBTree(0);
    char buff[256];
    TString key;
    unsigned long long data;
    while (std::cin >> buff) {
        if (buff[0] == '+') {
            std::cin >> key.string >> data;
            toLowerCase(key.string);
            std::cout << (tree->insert(key, data) ? "OK\n" : "Exist\n");
        } else
        if (buff[0] == '-') {
            std::cin >> key.string;
            toLowerCase(key.string);
            //std::cout << key.string << std::endl;
            std::cout << (tree->deleteKeyFromTree(key) ? "OK\n" : "NoSuchWord\n");
        } else
        if (buff[0] == '!') {
            std::cin >> buff;
            if (stringCompare(buff, "Save")) {
                std::cin >> buff;
                FILE* out = fopen(buff, "wb");
                if (out == nullptr) {
                    std::cout << "ERROR: Error opening file\n";
                } else {
                    std::cout << "OK\n";
                }
                tree->save(out);
                fclose(out);
            } else
            if (stringCompare(buff, "Load")) {
                std::cin >> buff;
                FILE* in = fopen(buff, "rb");
                if (in == nullptr) {
                    std::cout << "ERROR: Error opening file\n";
                } else {
                    if (buffTree->load(in)) {
                        std::cout << "OK\n";
                        tree = buffTree;
                    } else {
                        std::cout << "ERROR: File is corrupted or something\n";
                    }
                    fclose(in);
                }
            } else {
                std::cout << "ERROR: Unknown command\n";
            }
        } else {
            toLowerCase(buff);
            stringAssignment(key.string, buff);
            tree->search(key);
        }
    }
    if (tree != buffTree) {
        delete tree;
        delete buffTree;
    } else {
        delete tree;
    }

    return 0;
}
