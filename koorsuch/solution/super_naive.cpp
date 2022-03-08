#include <iostream>
#include <vector>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

struct TSegment {
    double l;
    double r;
    double h;
    TSegment(double left, double right, double height) {
        l = left;
        r = right;
        h = height;
    }
    ~TSegment() {}
};

int main() {
    std::vector<TSegment> vec;
    int n, m;
    double l, r, h;
    double x, y;
    int segCount = 0;

    std::cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        std::cin >> l >> r >> h;
        vec.push_back(TSegment(l, r, h));
    }

    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        segCount = 0;
        for (TSegment seg : vec) {
            if (seg.l <= x && seg.r >= x && seg.h > y) {
                //std::cout << h << " " << y << std::endl;
                ++segCount;
                //std::cout << seg.l << " " << seg.r << " " << seg.h << std::endl;
            }
        }
        std::cout << "The amount of segments above this point is " << segCount << std::endl;
    }

    auto end_ts = std::chrono::system_clock::now();
    uint64_t super_naive_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    
    std::cout << "Super naive time: " << super_naive_ts << DURATION_PREFIX << std::endl;

    return 0;
}