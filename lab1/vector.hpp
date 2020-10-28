#ifndef VECTOR_HPP
#define VECTOR_HPP


// struct TNode {
//     int Position;
//     int Key;
//     std::string Data;
//     struct TNode* Next;
// };

struct TNode1 {
    int Position;
    int Key;
    char* Data;
    struct TNode1* Next;
};

int stringAssignment(char* source, char* destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        ++i;
    }
    destination[i] = source[i];
    return 0;
}

int stringLength(char* string) {
    int i = 0;
    while (string[i] != '\0') {
        ++i;
    }

    return ++i;
}

int VectorPushBack(struct TNode1* root, int key, char* data) {
    while (root->Next != NULL) {
        root = root->Next;
    }
    root->Next = (struct TNode1*)malloc(sizeof(struct TNode1) + sizeof(char) * stringLength(data));
    if (root->Next != NULL) {
        root = root->Next;
        root->Position = 0;
        root->Key = key;
        root->Data = (char*)malloc(sizeof(char) * stringLength(data));
        stringAssignment(data, root->Data);
        root->Next = NULL;
    } else {
        return -1;
    }

    return 0;
}

int vectorClear(struct TNode1* vector) {
    struct TNode1* ptr;
    while (vector->Next != NULL) {
        ptr = vector->Next;
        free(vector->Data);
        free(vector);
        vector = ptr;
    }
    free(vector->Data);
    free(vector);

    return 0;
}



#endif

// class TVector {
// private:
//     unsigned int Size;
//     struct TNode Vector;
// public:
//     TVector() {
//
//     };
// };
