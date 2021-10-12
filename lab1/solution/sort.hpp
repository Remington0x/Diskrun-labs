#ifndef SORT_HPP
#define SORT_HPP

#include <iostream>
#include <string>
#include "vector.hpp"
#include "pair_lab1.hpp"

namespace sort {
    void counting_sort(TVector< TPair<int, std::string> >* vec) {
        typedef TPair<int, std::string> T_Pair;
        int keys[1000000];
        for (int i = 0; i < 1000000; ++i) {
            keys[i] = 0;
        }
        int num_count = vec->getSize();

        for (int i = 0; i < num_count; ++i) {
            ++keys[vec->at(i).first];
        }

        T_Pair* array_b = (T_Pair*)calloc(num_count, sizeof(T_Pair));
        for (int i = 0; i < num_count; ++i) {
            //std::cout << i << std::endl;
            array_b[i].first = -1;
            array_b[i].second = "\n";
        }

        for (int i = 1; i < 1000000; ++i) {
            keys[i] = keys[i] + keys[i - 1];
        }

        for (int i = 999999; i > 0; --i) {
            keys[i] = keys[i - 1];
        }

        keys[0] = 0;

        T_Pair buff;
        for (int i = 0; i < num_count; ++i) {
            buff = vec->at(i);
            array_b[keys[buff.first]].first = buff.first;
            array_b[keys[buff.first]].second = buff.second;
            ++keys[buff.first];
        }


        TVector< T_Pair > vec2;
        for (int i = num_count - 1; i >= 0; --i) {
            vec2.insert(T_Pair(array_b[i].first, array_b[i].second), 0);
        }

        *vec = vec2;
        free(array_b);

        return;
    }
}


#endif