#include <iostream>

struct Token {
    int key;
    char data[2048];
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

int main() {
    int keys[1000000];
    for (int i = 0; i < 1000000; ++i) {
        keys[i] = 0;
    }
    int num_count = 0;
    int int_buff = 0;
    char char_buff[2048];
    Token* array_a = nullptr;


    while (scanf("%d\t%s", &int_buff, char_buff) > 0) {
        ++keys[int_buff];
        ++num_count;
        array_a = (Token*)realloc(array_a, sizeof(Token) * num_count);
        array_a[num_count - 1].key = int_buff;
        stringAssignment(char_buff, array_a[num_count - 1].data);
    }

    // printf("NUM COUNT = %d\n", num_count);
    //
    // for (int i = 0; i < 100; ++i) {
    //     printf("%d ", keys[i]);
    // }
    // printf("\n\n");

    Token* array_b = (Token*)malloc(sizeof(Token) * num_count);
    for (int i = 0; i < num_count; ++i) {
        array_b[i].key = -1;
        for (int j = 0; j < 2048; ++j) {
            array_b[i].data[j] = '\0';
        }
    }

    for (int i = 1; i < 1000000; ++i) {
        keys[i] = keys[i] + keys[i - 1];
    }

    // for (int i = 0; i < 100; ++i) {
    //     printf("%d ", keys[i]);
    // }

    for (int i = 999999; i > 0; --i) {
        keys[i] = keys[i - 1];
    }

    // for (int i = 0; i < 100; ++i) {
    //     printf("%d ", keys[i]);
    // }

    keys[0] = 0;

    for (int i = 0; i < num_count; ++i) {
        array_b[keys[array_a[i].key]].key = array_a[i].key;
        stringAssignment(array_a[i].data, array_b[keys[array_a[i].key]].data);
        ++keys[array_a[i].key];
    }

    //Вывод
    //printf("NUM COUNT = %d\n", num_count);
    for (int i = 0; i < num_count; ++i) {
        // if (array_b[i].key != -1)
            printf("%06d\t%s\n", array_b[i].key, array_b[i].data);
    }

    free(array_a);
    free(array_b);

    return 0;
}
