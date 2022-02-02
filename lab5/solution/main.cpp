#include "suffix_tree.hpp"
#include <chrono>

using duration_t = std::chrono::microseconds;
//const std::string DURATION_SUFFIX = "us";

const char SENTINEL1 = '$';
const char SENTINEL2 = '*';

int main() {
    std::string s;
    std::cin >> s;
    std::string t = s + SENTINEL1;
    std::cin >> s;
    t = t + s + SENTINEL2;

    TSuffixTree st(t);
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    
    st.Find(SENTINEL1, SENTINEL2);
    auto end_ts = std::chrono::system_clock::now();

    
    uint64_t ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    std::cout << "Ukkonen algorithm time: " << ts << DURATION_SUFFIX << std::endl;
    return 0;
}