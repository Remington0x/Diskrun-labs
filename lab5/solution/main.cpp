#include "suffix_tree.hpp"

const char SENTINEL1 = '$';
const char SENTINEL2 = '*';

int main() {
    std::string s;
    std::cin >> s;
    std::string t = s + SENTINEL1;
    std::cin >> s;
    t = t + s + SENTINEL2;
    TSuffixTree st(t);
    
    //st.Print();
    //std::cout << "\n\n";
    st.Find(SENTINEL1, SENTINEL2);
    // std::cin >> s >> t;
    // std::vector<std::string> vec;
    // vec.push_back(s);
    // vec.push_back(t);
    // MySort(vec, 0, 1);
    // std::cout << vec[0] << std::endl << vec[1] << std::endl;
    //std::cout << st.RecMaxDepth(SENTINEL1, SENTINEL2, 0, 0) << std::endl;
    //std::cout << st.LexMinString(s.size()) << '\n';
    return 0;
}