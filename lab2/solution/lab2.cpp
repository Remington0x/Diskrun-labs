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
