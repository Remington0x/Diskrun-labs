#include <iostream>
#include <vector>
#include <limits>

const int IMAX = std::numeric_limits<int>::max();

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
    if (lengths[finish] == IMAX) {
        std::cout << "No solution\n";
    } else {
        std::cout << lengths[finish] << std::endl;
    }

    return 0;
}