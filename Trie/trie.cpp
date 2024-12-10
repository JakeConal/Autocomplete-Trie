#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>

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
    if (prefix.empty())
        return nullptr;

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

bool isLeaf(Node* node) {
    if (node == nullptr)
        return false;

    for (int i = 0; i < 26; i++)
        if (node->child[i] != nullptr)
            return false;
    
    return true;
}

bool removeWord(Node*& root, std::string word, int index) {
    if (root == nullptr)
        return false;
    
    if (index == word.size()) {
        if (root->isLast) 
            root->isLast = false;
        return isLeaf(root);
    }
    
    if (removeWord(root->child[word[index] - 'a'], word, index + 1)) {
        Node* temp = root->child[word[index] - 'a'];
        root->child[word[index] - 'a'] = nullptr;
        delete temp;
        temp = nullptr;
        return isLeaf(root);
    }
    
    return false;
}

void destroyTrie(Node* root) {
    if (root == nullptr)
        return;

    for (int i = 0; i < 26; i++) 
        if (root->child[i] != nullptr) 
            destroyTrie(root->child[i]);

    delete root;
    root = nullptr;
}

void start() {
    Node* trie = createTrie("words_alpha.txt");

    while (true) {
        system("cls");
        std::cout << "[1] Search\n";
        std::cout << "[2] Delete\n";
        std::cout << "[3] Insert\n";

        int option = getch();

        if (option == '1') {

            system("cls");

            int k = 0;
            std::cout << "Enter the number of words you want suggested: ";
            std::cin >> k;

            system("cls");
            std::cout << "search: ";

            char c;
            std::string curWord;
            
            std::vector<std::string> suggestList;
            std::string temp;

            while (true) {
                
                //Enter a word
                c = getch();
                if (c == 8 && !curWord.empty())
                    curWord.pop_back();
                else
                    if (c >= 'a' && c <= 'z')
                        curWord.push_back(c);
                else if (c == 27)
                    break;

                system("cls");

                std::cout << "search: " << curWord << '\n';

                Node* lastPrefixNode = findLastPrefixNode(trie, curWord);
                if (lastPrefixNode == nullptr) {
                    if (!curWord.empty())
                        std::cout << "not found\n";
                    continue;
                }

                suggestWord(lastPrefixNode, k, suggestList, temp);

                for (auto i : suggestList) {
                    std:: cout << curWord + i << '\n';
                }

                suggestList.clear();
            }
        }
        else if (option == '2') {
            system("cls");

            std::string word;
            std::cout << "Enter a word to remove: ";
            std::cin >> word;
            
            removeWord(trie, word, 0);

            std::cout << "Removed Successfully. Press any key to continue";

            char x = getch();
        }
        else if (option == '3') {
            system("cls");

            std::string word;
            std::cout << "Enter a word to insert to dictionary: ";
            std::cin >> word;

            insert(trie, word);

            std::cout << "Inserted successfully. Press any key to continue";

            char x = getch();
        }
    }
}

int main() {
    
    start();
    return 0;
}