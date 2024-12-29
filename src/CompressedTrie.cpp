#include "Header.h"
#include <fstream>

TrieNode::TrieNode(bool isEnd) {
    this->isEnd = isEnd;
}

CompressedTrie::CompressedTrie() {
    root = new TrieNode(false);
    root->key = "";
}

void CompressedTrie::createTrie(std::string fileName) {
    std::fstream dictionary(fileName, std::ios::in);
    std::string curWord;
    while (getline(dictionary, curWord)) {
        if (curWord.empty() || curWord.length() < 4) continue;
        insert(curWord);
    }
}

void CompressedTrie::insert(std::string& word) {
    TrieNode* curNode = root;
    int i = 0;

    while (i < word.length()) {
        char curChar = word[i];

        if (curNode->child.find(curChar) != curNode->child.end()) {
            TrieNode* childNode = curNode->child[curChar];
            std::string childKey = childNode->key;
            int j = 0;
            
            while (i < word.length() && j < word.length() && word[i] == childKey[j]) {
                ++i;
                ++j;
            }

            if (j == childKey.length()) {
                curNode = childNode;
            } 
            else {
                if (i == word.length()) {
                    TrieNode* newNode = new TrieNode(true);
                    std::string remainKey = childKey.substr(j);

                    curNode->child[curChar] = newNode;
                    newNode->child[remainKey[0]] = childNode;

                    newNode->key = childKey.substr(0, j);
                    childNode->key = remainKey;                    
                }
                else {
                    std::string remainKey = childKey.substr(j);
                    std::string remainWord = word.substr(i);

                    TrieNode* newNode = new TrieNode(false);
                    TrieNode* newWordNode = new TrieNode(true);

                    curNode->child[curChar] = newNode;
                    newNode->child[remainKey[0]] = childNode;
                    newNode->child[remainWord[0]] = newWordNode;

                    newNode->key = childKey.substr(0, j);
                    childNode->key = remainKey;
                    newWordNode->key = remainWord;
                }
                return;
            } 
        }  
        else {
            TrieNode* newNode = new TrieNode(true);
            newNode->key = word.substr(i);
            curNode->child[curChar] = newNode;
            return;
        }
    }
}

bool CompressedTrie::remove(std::string& word) {
    TrieNode* curNode = root;
    std::vector<std::pair<TrieNode*, char>> path; 
    int i = 0;

    while (i < word.length()) {
        if (curNode->child.find(word[i]) == curNode->child.end()) {
            return false; 
        }

        path.push_back({curNode, word[i]});
        TrieNode* childNode = curNode->child[word[i]];
        std::string childKey = childNode->key;
        int j = 0;

        while (i < word.length() && j < childKey.length() && word[i] == childKey[j]) {
            ++i;
            ++j;
        }

        if (j == childKey.length()) {
            curNode = childNode;
        } else {
            return false; 
        }
    }

    if (!curNode->isEnd) {
        return false; 
    }

    curNode->isEnd = false;

    
    for (int k = path.size() - 1; k >= 0; --k) {
        TrieNode* parentNode = path[k].first;
        char edgeChar = path[k].second;
        TrieNode* node = parentNode->child[edgeChar];

        if (node->isEnd || !node->child.empty()) {
            if (!node->isEnd && node->child.size() == 1) {
                auto child = node->child.begin();
                node->key += child->second->key;
                node->isEnd = child->second->isEnd;
                node->child = std::move(child->second->child);
                delete child->second;
            }
            break;
        }

        parentNode->child.erase(edgeChar);
        delete node;
        node = nullptr;
    }

    return true;
}

void CompressedTrie::destroy(TrieNode* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->child) {
        destroy(child.second);
    }

    delete node;
    node = nullptr;
}

std::pair<TrieNode*, std::string> CompressedTrie::search(std::string& prefix, long long& countComparison) {
    TrieNode* curNode = root;
    int i = 0;

    while (++countComparison && i < prefix.length()) {
        if (++countComparison && curNode->child.find(prefix[i]) == curNode->child.end())
            return {nullptr, ""};

        TrieNode* childNode = curNode->child[prefix[i]];
        std::string childKey = childNode->key;
        int j = 0;

        while (++countComparison && i < prefix.length() && ++countComparison && j < childKey.length() && ++countComparison && prefix[i] == childKey[j]) {
            ++i;
            ++j;
        }

        if (++countComparison && j == childKey.length()) {
            curNode = childNode;
        } else if (++countComparison && i == prefix.length()) {
            return {childNode, childKey.substr(j)};
        } else {
            return {nullptr, ""};
        }
    }

    return {curNode, ""};
}

void suggestWord(TrieNode* curNode, std::vector<std::string>& curList, std::string prefix, std::string curWord, int num, long long& countComparison) {
    if (++countComparison && curNode->isEnd) {
        curList.push_back(prefix + curWord);
    }

    if (++countComparison && curList.size() > num) {
        return;
    }

    for(auto child : curNode->child) {
        if (++countComparison && curList.size() <= num) {
            suggestWord(child.second, curList, prefix, curWord + child.second->key, num, countComparison);
        }
    }
}

std::vector<std::string> autocomplete(CompressedTrie trie, std::string prefix, int num, long long& countComparison) {
    auto [prefixNode, remainingKey] = trie.search(prefix, countComparison);
    std::vector<std::string> suggestList;

    if (++countComparison && prefixNode != nullptr) {
        suggestWord(prefixNode, suggestList, prefix, remainingKey, num, countComparison);
    }

    return suggestList;
}