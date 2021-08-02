#include <iostream>

struct TString {
    char string[256];
};

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
    // enum report {
    //     OK = 0,
    //
    // }

    unsigned long long recSearch(TNode* node, TString& key) {
        int i = 0;
        for (i = 0; i < keyCount; ++i) {
            if (node->keys[i] == key) {
                return (node->data[i]);
            } else
            if (node->keys[i] > key) {
                return recSearch(node->children[i], key);
            }
        }
        if (node->keys[i - 1] < key) {
            return recSearch(node->children[i + 1], key);
        }
        isSuchWord = false;
        return 0;
    }

    void nodeAddKey(TNode* node, TString& key, unsigned long long& data) {

    }

    void nodeSplit(TNode* node) {
        if (keyCount < (2 * t - 1)) {
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
            }
            newNode->children[i - t] = node->children[i];
        }

        if (node != root) {
            newNode->parent = node->parent;
            //parent node manipulations
            node->parent->keyCount += 1;
            node->parent->keys = (TString*)realloc(node->parent->keys, sizeof(TString) * node->parent->keyCount);
            node->parent->data = (unsigned long long*)realloc(node->parent->data, sizeof(unsigned long long) * node->parent->keyCount);
            node->parent->children = (TNode**)realloc(node->parent->children, sizeof(TNode*) * (node->parent->keyCount + 1));
            i = 0;
            while (node->parent->keys[i] < node->keys[t - 1]) {
                ++i;
            }
            insertInArray(node->parent->keys, keyCount, node->keys[t - 1], i);
            insertInArray(node->parent->data, keyCount, node->data[t - 1], i);
            insertInArray(node->parent->children, keyCount + 1, newNode, i + 1);

        } else {    //if node == root
            TNode* newRoot = (TNode*)malloc(sizeof(TNode));
            node->parent = newRoot;
            newNode->parent = newRoot;

            newRoot->isLeaf = false;
            newRoot->keyCount = 1;
            newRoot->keys = (TString*)malloc(node->parent->keys, sizeof(TString) * newRoot->keyCount);
            newRoot->keys[0] = node->keys[t - 1];
            newRoot->data = (unsigned long long*)malloc(node->parent->data, sizeof(unsigned long long) * newRoot->keyCount);
            newRoot->data[0] = node->data[t - 1];
            newRoot->children = (TNode**)malloc(newRoot->children, sizeof(TNode*) * (newRoot->keyCount + 1));
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
            for (i = 0; i < keyCount; ++i) {
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
            if (keyCount < (2 * t - 1)) { //leaf is not full
                nodeAddKey(node, key, data);
            } else {    //time to split
                nodeSplit(node);    //split
                recInsert(node->parent, key, data);    //add
            }
        }
    }

public:
    TBTree() {}
    ~TBTree() {}

    unsigned long long search(TString& key) {
        isSuchWord = true;
        unsigned long long result = recSearch(root, key);
        if (isSuchWord) {
            return result;
        } else {
            std::cout << "NoSuchWord\n";
        }
    }

    //returns true if insertion successful, false if word already exists
    bool insert(TString& key, unsigned long long& data) {
        doesAlreadyExist = false;
        recInsert(root, key, data);
    }


};

//works with lower-cased chars
bool operator<(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
        if (a.string[i] < b.string[i]) {
            return true;
        } else
        if (a.string[i] > b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] == '\0' && b.string[i] != '\0') {
        return true;
    } else {
        return false;
    }
}

bool operator>(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
        if (a.string[i] > b.string[i]) {
            return true;
        } else
        if (a.string[i] < b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] != '\0' && b.string[i] == '\0') {
        return true;
    } else {
        return false;
    }
}

bool operator==(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
        if (a.string[i] != b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] == '\0' && b.string[i] == '\0') {
        return true;
    } else {
        return false;
    }
}

int main() {
    std::cout << "Enter a and b\n";
    TString a, b;
    std::cin >> a.string >> b.string;
    std::cout << "a < b = " << (a < b) << std::endl;
    std::cout << "a > b = " << (a > b) << std::endl;
    std::cout << "a == b = " << (a == b) << std::endl;











    return 0;
}
