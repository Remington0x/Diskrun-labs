#include <iostream>
#include <string>

int main() {
	int b[1000000];
    for (int i = 0; i < 1000000; ++i) {
        b[i] = 0;
    }

    int key;
    std::string string;

    scanf("%d\t%s", &key, &string);
    b[key] += 1;
    struct TNode* vectorA = (struct TNode*)malloc(sizeof(struct TNode));
    vectorA->Position = 0;
    vectorA->Key = key;
    vectorA->Data = data;
    vectorA->Next = NULL;

	while (scanf("%d\t%s", &key, &string) > 0) {
        b[key] += 1;
        VectorPushBack(vectorA, key, string);
    }

    int buff = b[0];
    int n;
    b[0] = 0;
    for (int i = 1; i < 1000000; ++i) {
        n = b[i];
        b[i] = buff;
        buff += n;
    }

    


}
