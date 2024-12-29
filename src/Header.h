#pragma once
#include <unordered_map>
#include <string>
#include <vector>

//Compressed Trie;
struct TrieNode {
    std::string key;
    std::unordered_map<char, TrieNode*> child;
    bool isEnd;

    TrieNode(bool isEnd);
};

struct CompressedTrie {
    TrieNode* root;

    CompressedTrie();

    void createTrie(std::string fileName);
    void insert(std::string& word);
    bool remove(std::string& word);
    void destroy(TrieNode* node);
    std::pair<TrieNode*, std::string> search(std::string& prefix, long long& comparisonCount);
};

void suggestWord(TrieNode* curNode, std::vector<std::string>& curList, std::string prefix, std::string curWord, int num, long long& comparisonCount);
std::vector<std::string> autocomplete(CompressedTrie Trie, std::string prefix, int num, long long& comparisonCount);

//Brute Force algorithm
std::vector<std::string> autocomplete(std::string fileName, std::string prefix, int num, long long& comparisonCount);

//Menu
void displayMenu();
void search(CompressedTrie trie);
void insert(CompressedTrie& trie);
void remove(CompressedTrie& trie);
