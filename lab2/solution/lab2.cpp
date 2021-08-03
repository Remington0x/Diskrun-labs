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
            std::cout << (tree->insert(key, data) ? "OK\n" : "Exist\n");
        } else
        if (buff[0] == '-') {

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
