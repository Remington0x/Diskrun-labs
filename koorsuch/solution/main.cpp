//#include "tree.hpp"

#include "tree_sp.hpp"

#include <algorithm>
#include <string>
#include <chrono>
#include <string>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

struct TStrip {
    double XLeft;
    std::shared_ptr<TNode> TreeRoot;
    TStrip(double l, std::shared_ptr<TNode> root) {
        XLeft = l;
        TreeRoot = root;
    }
    ~TStrip() {}
};

struct TSegment {
    double L;
    double R;
    double H;
    TSegment() {}
    TSegment(double left, double right, double height) {
        L = left;
        R = right;
        H = height;
    }
    ~TSegment() {}
    bool operator < (const TSegment & b) {
        return this->L < b.L;
    }
};

template<class T>
void vecInsert(T element, unsigned int pos, std::vector<T> & vec) {
    if (pos > vec.size() + 1) {
        throw std::logic_error("pos > vec.size + 1");
    }
    vec.resize(vec.size() + 1);
    for (unsigned int i = vec.size() - 1; i > pos; --i) {
        vec[i] = vec[i - 1];
    }
    vec[pos] = element;
}

void addToQueue(TSegment seg, std::vector<TSegment> & queue) {
    unsigned int i;
    for (i = 0; i < queue.size(); ++i) {
        if (seg.R < queue.at(i).R) {
            vecInsert(seg, i, queue);
            break;
        }
    }
    if (i == queue.size()) {
        vecInsert(seg, i, queue);
    }
}

int countSubtree(TNode* node) {
    int counter = 0;
    if (node != nullptr) {
        counter += 1;
        counter += countSubtree(node->Left.get());
        counter += countSubtree(node->Right.get());
    }
    return counter;
}


int main() {
    auto tree = std::make_shared<TTree>();
    std::vector<TStrip> strips;
    std::vector<TSegment> vec;
    std::vector<TSegment> queue;

    int n, m;
    double l, r, h;
    double x, y;

    std::cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        std::cin >> l >> r >> h;
        vec.push_back(TSegment(l, r, h));
    }
    std::sort(vec.begin(), vec.end());


    // std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();

    std::shared_ptr<TNode> newRoot;
    
    while (!vec.empty() || !queue.empty()) {
        if (!queue.empty()) {
            if (vec.empty() || queue.at(0).R < vec.at(0).L) {  //segment queue[0] ends

                unsigned int vsize = tree->Values.size();
                unsigned int qsize = queue.size();

                newRoot = tree->RemNode(queue.at(0).H);
                strips.push_back(TStrip(queue.at(0).R, newRoot));
                queue.erase(queue.begin());

            } else 
            if (queue.at(0).R > vec.at(0).L) {  //segment vec[0] starts
                newRoot = tree->AddNode(vec.at(0).H);
                strips.push_back(TStrip(vec.at(0).L, newRoot));
                addToQueue(vec.at(0), queue);
                vec.erase(vec.begin());

            } else {    //queue[0] ends and vec[0] starts
                tree->RemNode(queue.at(0).H);

                newRoot = tree->AddNode(vec.at(0).H);

                strips.push_back(TStrip(vec.at(0).L, newRoot));
                queue.erase(queue.begin());
                addToQueue(vec.at(0), queue);
                vec.erase(vec.begin());
            }
        } else {    //queue is empty
            newRoot = tree->AddNode(vec.at(0).H);

            strips.push_back(TStrip(vec.at(0).L, newRoot));
            addToQueue(vec.at(0), queue);
            vec.erase(vec.begin());
        }
    }

    //ready to query?
    // int cnt = 0;
    // for (TStrip st : strips) {
    //     std::cout << "POLOSA N " << cnt;
    //     std::cout << ", l = " << st.XLeft << std::endl;
    //     ++cnt;
    // }

    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        //1. determine strip
        //strips[0] -- first strip with >0 segments
        //use binary search
        if (x < strips.at(0).XLeft) {
            std::cout << 0 << std::endl;
            continue;
        }

        l = 0;
        r = strips.size() - 1;
        double mid = r - l / 2;
        bool flag = true;
        while (flag) {
            if (x < strips.at(mid).XLeft) {
                r = mid - 1;
            } else 
            if (x > strips.at(mid).XLeft) {
                l = mid + 1;
            } else {
                flag = false;
            }
            if (r - l <= 1) {
                mid = l;
                flag = false;
            } else {
                mid = r - l / 2;
            }
            if (mid == strips.size()) {
                flag = false;
                --mid;
            }
        }
        
        //2. determine branch of tree
        // std::cout << "Mid = " << mid << std::endl;
        // tree->PrintTree(mid);

        flag = true;
        auto curNode = tree->Roots.at(mid);
        if (curNode == nullptr) {
            std::cout << 0 << std::endl;
            continue;
        }
        int segCount = 0;
        while (flag) {
            if (y < curNode->Height) {
                segCount += 1;
                segCount += countSubtree(curNode->Right.get());
                if (curNode->Left != nullptr) {
                    curNode = curNode->Left;
                } else {
                    flag = false;
                }
            } else
            if (y >= curNode->Height) {
                if (curNode->Right != nullptr) {
                    curNode = curNode->Right;
                } else {
                    flag = false;
                }
            }   

        std::cout << segCount << std::endl;
        }
    }

    //auto end_ts = std::chrono::system_clock::now();
    //uint64_t persistent_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    
    //std::cout << "Persistent time: " << persistent_ts << DURATION_PREFIX << std::endl;

    return 0;
}