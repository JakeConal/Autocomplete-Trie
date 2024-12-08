#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct Node {
    bool isLast;
    Node* child[26];
};

Node* createNode() {
    Node* p = new Node;
    p->isLast = false;

    for (int i = 0; i < 26; i++) 
        p->child[i] = nullptr;
    
    return p;
}

void insert(Node*& root, std::string word) {
    
    Node* curNode = root;

    for (auto i : word) {
        if (curNode->child[i - 'a'] == nullptr) 
            curNode->child[i - 'a'] = createNode();
        curNode = curNode->child[i - 'a'];
    }

    curNode->isLast = true;
}

Node* createTrie(std::string fileName) {
    std::fstream dictionary(fileName, std::ios::in);

    Node* root = createNode();

    std::string word;
    while(getline(dictionary, word)) {
        insert(root, word);
    }
    
    dictionary.close();
    return root;
}

Node* findLastPrefixNode(Node* root, std::string prefix) {
    Node* curNode = root;
    
    for (auto i : prefix) {
        if (curNode->child[i - 'a'] == nullptr)
            return nullptr;
        curNode = curNode->child[i - 'a'];
    }

    return curNode;
}

void suggestWord(Node* curNode, int k, std::vector<std::string>& wordList, std::string curWord) {
    if (curNode->isLast) {
        wordList.push_back(curWord);
    }

    for (int i = 0; i < 26; i++) {
        if (curNode->child[i] != nullptr && wordList.size() <= k) {
            curWord.push_back(i + 'a');
            suggestWord(curNode->child[i], k, wordList, curWord);
            curWord.pop_back();
        }
    }
}



int main() {
    Node* trie = createTrie("words_alpha.txt");
    Node* lastPrefixNode = findLastPrefixNode(trie, "a");
    std::vector<std::string> wordList;
    std::string curWord;
    suggestWord(lastPrefixNode, 10, wordList, curWord);
    for (auto i : wordList)
        std::cout << i << '\n';

    return 0;
}