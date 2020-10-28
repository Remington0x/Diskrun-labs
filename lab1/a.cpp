#include <iostream>
#include "vector.hpp"

int main() {
	int b[1000000];
    for (int i = 0; i < 1000000; ++i) {
        b[i] = 0;
    }

    int key;
    char string[2048];

    scanf("%d\t%s", &key, &string);
    b[key] += 1;
	//std::cout << "scanf done\n";
    struct TNode1* vectorA = (struct TNode1*)malloc(sizeof(struct TNode1) + stringLength(string) * sizeof(char));
	//std::cout << "malloc done\n";
    vectorA->Position = 0;
    vectorA->Key = key;
	//std::cout << "position/key done\n";

    vectorA->Data = (char*)malloc(stringLength(string) * sizeof(char));
	stringAssignment(string, vectorA->Data);
	//std::cout << "string done\n";

    vectorA->Next = NULL;
	//std::cout << "init done\n";

	while (scanf("%d\t%s", &key, &string) > 0) {
        b[key] += 1;
		//std::cout << "scanf done\n";
        VectorPushBack(vectorA, key, string);
		//std::cout << "push back done\n";
    }

    int buff = b[0];
    int n;
    b[0] = 0;
    for (int i = 1; i < 1000000; ++i) {
        n = b[i];
        b[i] = buff;
        buff += n;
    }

    //now the main sorting appears
	struct TNode1* ptrA = vectorA;
	struct TNode1* vectorB = NULL;
	struct TNode1* ptrB = vectorB;
	struct TNode1* buffptr = NULL;
	int pos;

	while (ptrA != NULL) {	//took vectorA[i]
		//three (4) cases
		//2. a[i] is left to first
		//3. a[i] is in the middle
		//4. a[i] is right to last
		//1. *b is empty

		pos = b[ptrA->Key];
		ptrB = vectorB;

		if (vectorB == NULL) {	//1.
			vectorB = (struct TNode1*)malloc(sizeof(struct TNode1) + sizeof(char) * stringLength(ptrA->Data));
			vectorB->Position = pos;
		    vectorB->Key = ptrA->Key;
			vectorB->Data = (char*)malloc(sizeof(char) * stringLength(ptrA->Data));
		    stringAssignment(ptrA->Data, vectorB->Data);
		    vectorB->Next = NULL;
		} else if (pos < ptrB->Position) {	//2.
			vectorB = (struct TNode1*)malloc(sizeof(struct TNode1) + sizeof(char) * stringLength(ptrA->Data));
			vectorB->Position = pos;
			vectorB->Key = ptrA->Key;
			vectorB->Data = (char*)malloc(sizeof(char) * stringLength(ptrA->Data));
			stringAssignment(ptrA->Data, vectorB->Data);
			vectorB->Next = ptrB;
		} else {
			while (ptrB->Next != NULL) {	//3.
				if (ptrB->Next->Position > pos) {
					buffptr = ptrB->Next;
					ptrB->Next = (struct TNode1*)malloc(sizeof(struct TNode1) + sizeof(char) * stringLength(ptrA->Data));
					ptrB->Next->Position = pos;
				    ptrB->Next->Key = ptrA->Key;
					ptrB->Next->Data = (char*)malloc(sizeof(char) * stringLength(ptrA->Data));
				    stringAssignment(ptrA->Data, ptrB->Next->Data);
				    ptrB->Next->Next = buffptr;
					break;
				}
				ptrB = ptrB->Next;
			}

			if (ptrB->Next == NULL) {	//4.
				ptrB->Next = (struct TNode1*)malloc(sizeof(struct TNode1) + sizeof(char) * stringLength(ptrA->Data));
				ptrB->Next->Position = pos;
				ptrB->Next->Key = ptrA->Key;
				ptrB->Next->Data = (char*)malloc(sizeof(char) * stringLength(ptrA->Data));
				stringAssignment(ptrA->Data, ptrB->Next->Data);
				ptrB->Next->Next = NULL;
			}
		}
		ptrA = ptrA->Next;

	}	//end of while

	ptrB = vectorB;
	while (ptrB != NULL) {
		printf("%d\t%s\n", ptrB->Key, ptrB->Data);
		ptrB = ptrB->Next;
	}

	vectorClear(vectorA);
	vectorClear(vectorB);


}
