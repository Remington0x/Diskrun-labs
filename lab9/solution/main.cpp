#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

const int IMAX = std::numeric_limits<int>::max();
using duration_t = std::chrono::microseconds;

struct TEdge {
    int r;
    int weight;
    TEdge(int vertex, int w) {
        r = vertex;
        weight = w;
    }
    ~TEdge() {};
};

int main() {
    int n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    --start;
    --finish;

    std::vector<std::vector<TEdge>> vertices(n);

    int l, r, w;
    for (int i = 0; i < m; ++i) {
        std::cin >> l >> r >> w;
        vertices[l - 1].push_back(TEdge(r - 1, w));
    }

    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();

    //Bellman-Ford algorithm
    std::vector<int> lengths(n, IMAX);
    lengths[start] = 0;
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            for (TEdge edge : vertices[i]) {
                if (lengths[i] + edge.weight < lengths[edge.r]) {
                    lengths[edge.r] = lengths[i] + edge.weight;
                }
            }
        }
    }

    auto end_ts = std::chrono::system_clock::now();
    uint64_t b_f_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    //std::cout << "Bellman-Ford time: " << b_f_ts << "us\n";
    if (lengths[finish] == IMAX) {
        std::cout << "No solution\n";
    } else {
        std::cout << lengths[finish] << std::endl;
    }

    return 0;
}