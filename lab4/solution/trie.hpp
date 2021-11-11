#ifndef TRIE_HPP
#define TRIE_HPP

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <memory>
#include <string>

class TTrie;

class TTrieNode {
public:
    friend class TTrie;
    TTrieNode();
    ~TTrieNode() {};
private:
    std::map<unsigned long, TTrieNode *> to;
    TTrieNode* linkFail;
    std::vector<int> out;
};

class TTrie {
public:
    TTrie();
    ~TTrie();
    void Create(const std::vector<std::string> &);
    void Search(const std::vector<unsigned long> &, const int &, std::vector<std::pair<int, int>> &);
private:
    TTrieNode *root;
    std::vector<int> lensPatterns;
    int woJoker;
    void CreateLinks();
};

#endif