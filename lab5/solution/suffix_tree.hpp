#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

class TSuffixTree {
private:
    struct TEdge {
        int Left;
        std::shared_ptr<int> Right;
        int IdTo;

        TEdge(int l, std::shared_ptr<int> r, int id) : Left(l), Right(r), IdTo(id) {}
    };

    std::string DataString;
    std::vector< std::vector< TSuffixTree::TEdge > > Data;
    std::vector<int> SuffixPtr;
    std::vector<int> PathSize;
public:
    TSuffixTree(const std::string & s);
    void Print();
    void Find(const char, const char);
    int RecMarker(const char, const char, const int, const int, int &, std::vector<int> &);
    void RecCatchStrings(const int, const int, const int, const std::string, std::vector<int> &, std::vector<std::string> &);
};

TSuffixTree::TSuffixTree(const std::string & s) : DataString(s), Data(s.size() * 2), SuffixPtr(s.size() * 2), PathSize(s.size() * 2) {
    bool newVertex = false;
    std::shared_ptr<int> end(new int);
    *end = 0;
    int curVertex = 0;
    int l = 0;
    int vertexId = 0;
    int curEq = 0;
    int passEq = 0;
    int curEdgeId = -1;
    for (size_t i = 0; i < s.size(); ++i) {
        ++*end;
        int lastCreatedVertexId = 0;
        int nextCreatedVertexId = -1;
        while (l < *end) {
            if (curEdgeId == -1) {
                int nextEdgeId = -1;
                for (size_t j = 0; j < Data[curVertex].size(); ++j) {
                    if (DataString[Data[curVertex][j].Left] == DataString[l + passEq + curEq]) {
                        nextEdgeId = j;
                        curEq = 1;
                        break;
                    }
                }
                if (nextEdgeId == -1) {
                    newVertex = true;
                }
                curEdgeId = nextEdgeId;
            } else {
                if (DataString[Data[curVertex][curEdgeId].Left + curEq] == DataString[l + curEq + passEq]) {
                    ++curEq;
                } else {
                    newVertex = true;
                }
            }
            if (curEq > 0 and Data[curVertex][curEdgeId].Left + curEq == *Data[curVertex][curEdgeId].Right) {
                curVertex = Data[curVertex][curEdgeId].IdTo;
                curEdgeId = -1;
                passEq = passEq + curEq;
                curEq = 0;
            }
            if (newVertex) {
                ++vertexId;
                if (curEq == 0 and curEdgeId == -1) {
                    Data[curVertex].push_back(TSuffixTree::TEdge(l + passEq, end, vertexId));
                } else {
                    nextCreatedVertexId = vertexId;
                    TEdge curEdge = Data[curVertex][curEdgeId];
                    std::shared_ptr<int> newRightBorder(new int);
                    *newRightBorder = Data[curVertex][curEdgeId].Left + curEq;
                    Data[curVertex][curEdgeId].Right = newRightBorder;
                    Data[curVertex][curEdgeId].IdTo = vertexId;
                    curEdge.Left = *newRightBorder;

                    Data[vertexId].push_back(curEdge);
                    ++vertexId;
                    Data[vertexId - 1].push_back(TSuffixTree::TEdge(l + curEq + passEq, end, vertexId));
                    PathSize[vertexId - 1] = PathSize[curVertex] + *Data[curVertex][curEdgeId].Right - Data[curVertex][curEdgeId].Left;

                    if (lastCreatedVertexId > 0) {
                        SuffixPtr[lastCreatedVertexId] = nextCreatedVertexId;
                    }
                    lastCreatedVertexId = nextCreatedVertexId;
                }
                int nextCurVertex = SuffixPtr[curVertex];
                int nextPassEq = PathSize[nextCurVertex];
                int nextCurEq = curEq + passEq - nextPassEq - 1;
                int nextEdgeId = -1;
                while (nextCurEq > 0) {
                    for (size_t j = 0; j < Data[nextCurVertex].size(); ++j) {
                        if (DataString[Data[nextCurVertex][j].Left] == DataString[l + 1 + nextPassEq]) {
                            nextEdgeId = j;
                            break;
                        }
                    }
                    int curRight = *Data[nextCurVertex][nextEdgeId].Right;
                    int curLeft = Data[nextCurVertex][nextEdgeId].Left;
                    if (nextEdgeId != -1 and curRight - curLeft <= nextCurEq) {
                        nextPassEq = nextPassEq + curRight - curLeft;
                        nextCurEq = nextCurEq - curRight + curLeft;
                        nextCurVertex = Data[nextCurVertex][nextEdgeId].IdTo;
                        nextEdgeId = -1;
                    } else {
                        break;
                    }
                }
                if (nextEdgeId != -1) {
                    curEq = nextCurEq;
                } else {
                    curEq = 0;
                }
                curEdgeId = nextEdgeId;
                curVertex = nextCurVertex;
                passEq = nextPassEq;
                ++l;
                newVertex = false;
            } else {
                if (i < s.size() - 1) {
                    break;
                }
            }
        }
    }
}
//snachala razmechaem vse vershiny
//kidaem v massiv
//potom hz

void TSuffixTree::Print() {
    for (long unsigned int i = 0; i < Data.size(); ++i) {
        for (long unsigned int j = 0; j < Data[i].size(); ++j) {
            for (int k = Data[i][j].Left; k < *Data[i][j].Right; ++k) {
                std::cout << DataString[k];
            }
            std::cout << " " << Data[i][j].IdTo << "\t";
        }
        std::cout << SuffixPtr[i];
        std::cout << std::endl;
    }
}

int StringComparison(std::string & str1, std::string & str2) {
    size_t minSize = std::min(str1.size(), str2.size());
    for (size_t i = 0; i < minSize; ++i) {
        if (str1[i] < str2[i]) {
            return -1;
        } else 
        if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

void MyInsert(std::vector<std::string> & ans, std::string str, int pos) {
    ans.insert(ans.end(), ans.back());
    size_t i;
    for (i = ans.size() - 2; i > (size_t)pos; --i) {
        ans[i] = ans.at(i - 1);
    }
    ans[i] = str;
    return;
}

void MyErase(std::vector<std::string> & ans, int pos) {
    for (size_t i = (size_t)pos; i < ans.size() - 2; ++i) {
        ans[i] = ans.at(i + 1);
    }
    ans.erase(ans.end());
    return;
}

void MySort(std::vector<std::string> & ans, int l, int r) {
    if (r - l + 1 <= 1) {
        return;
    }
    int basePos = l;
    std::string base = ans[l];
    for (int i = l + 1; i <= r; ++i) {
        if (StringComparison(ans[i], base) == -1) {
            std::string buff = ans[i];
            MyErase(ans, i);
            MyInsert(ans, buff, i);
            ++basePos;
        }
    }
    MySort(ans, l, basePos);
    MySort(ans, basePos + 1, r);
    return;
}

void TSuffixTree::Find(const char SENTINEL1, const char SENTINEL2) {
    int maxDepth = 0;
    std::vector<int> ids;
    RecMarker(SENTINEL1, SENTINEL2, 0, 0, maxDepth, ids);
    std::cout << maxDepth << std::endl;
    
    if (maxDepth > 0) {
        std::vector<std::string> answers;
        RecCatchStrings(maxDepth, 0, 0, "", ids, answers);
        //std::sort(answers.begin(), answers.end(), StringComparison);
        MySort(answers, 0, answers.size() - 1);
        for (size_t i = 0; i < answers.size(); ++i) {
            std::cout << answers.at(i) << "\n";
        }
    }
}

void TSuffixTree::RecCatchStrings(const int maxDepth, const int depth, const int id, const std::string str, std::vector<int> & ids, std::vector<std::string> & answers) {
    if (depth == maxDepth && id == ids.at(0)) {
        ids.erase(ids.begin());
        answers.push_back(str);
        return;
    }

    for (size_t i = 0; i < Data[id].size(); ++i) {
        std::string newStr = str;
        for (int j = Data[id][i].Left; j < *Data[id][i].Right; ++j) {
            newStr += DataString[j];
        }
        RecCatchStrings(maxDepth, depth + *Data[id][i].Right - Data[id][i].Left, Data[id][i].IdTo, newStr, ids, answers);
        
        if (ids.empty()) {
            return;
        }
    }

    return;
}

//0 if first
//1 if second
//2 if both
int TSuffixTree::RecMarker(const char SENTINEL1, const char SENTINEL2, const int depth, const int id, int & maxDepth, std::vector<int> & answers) {
    bool isFirst = false;
    bool isSecond = false;
    int mark;
    for (size_t i = 0; i < Data[id].size(); ++i) {
        //std::cout << "Data[id].size() = " << Data[id].size() << std::endl;
        //std::cout << "Going to ID " << Data[id][i].IdTo << "\n";
        mark = RecMarker(SENTINEL1, SENTINEL2, depth + *Data[id][i].Right - Data[id][i].Left, Data[id][i].IdTo, maxDepth, answers);

        
        if (mark == 0) {
            isFirst = true;
        } else 
        if (mark == 1) {
            isSecond = true;
        } else 
        if (mark == 2) {
            isFirst = true;
            isSecond = true;
            //break;
        }

        for (int j = Data[id][i].Left; j < *Data[id][i].Right; ++j) {
            if (DataString[j] == SENTINEL1) {
                isFirst = true;
                break;
            } else 
            if (DataString[j] == SENTINEL2) {
                isSecond = true;
                break;
            }
        }
    }
    //std::cout << "ID = " << id << " , isFirst = " << isFirst << " isSecond = " << isSecond << std::endl;
    if (isFirst && isSecond) {
        if (depth > maxDepth) {
            maxDepth = depth;
            answers.clear();
            answers.push_back(id);
            return 2;
        } else 
        if (depth == maxDepth) {
            answers.push_back(id);
            return 2;
        } else {
            return 2;
        }
    } else 
    if (isFirst) {
        return 0;
    } else 
    if (isSecond) {
        return 1;
    } else {
        return -1;
    }
    std::cout << isFirst << isSecond << std::endl;
    throw std::logic_error("Control reaches this somehow");
    return -2;
}

#endif /* SUFFIX_TREE_HPP */