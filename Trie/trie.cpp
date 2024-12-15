#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <chrono>

//TRIE NODE 
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

//INSERT A WORD TO TRIE
void insert(Node*& root, std::string word) {
    
    Node* curNode = root;

    for (auto i : word) {
        if (curNode->child[i - 'a'] == nullptr) 
            curNode->child[i - 'a'] = createNode();
        curNode = curNode->child[i - 'a'];
    }

    curNode->isLast = true;
}

//CREATE A TRIE FORM A DICTIONARY
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

//FIND ALL WORDS CONTAINING A GIVEN PREFIX USING TRIE

//find the node of the last character of the given prefix
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

//back-tracking to find k word containing the give prefix 
void suggestWord(Node* curNode, int k, std::vector<std::string>& wordList, std::string prefix, std::string curWord) {
    if (curNode->isLast) {
        wordList.push_back(prefix + curWord);
    }

    for (int i = 0; i < 26; i++) {
        if (curNode->child[i] != nullptr && wordList.size() <= k) {
            curWord.push_back(i + 'a');
            suggestWord(curNode->child[i], k, wordList, prefix, curWord);
            curWord.pop_back();
        }
    }
}

std::vector<std::string> autocompleteWord(Node* root, std::string prefix, int k) {
    std::vector<std::string> suggestWordList;

    Node* lastPrefixNode = findLastPrefixNode(root, prefix);

    if (lastPrefixNode == nullptr) 
        return suggestWordList;
    
    std::string temp;
    suggestWord(lastPrefixNode, k, suggestWordList, prefix, temp);

    return suggestWordList;
}

//FIND ALL WORDS CONTAINING A GIVEN PREFIX USING BRUTE-FORCE

std::vector<std::string> autocompleteWord(std::string dictionary, std::string prefix, int k) {
    std::fstream wordList(dictionary, std::ios::in);
    std::vector<std::string> suggestWordList;
    std::string word;

    if (prefix.empty())
        return suggestWordList;

    while (getline(wordList, word)) {
        if (!word.empty() && word.find(prefix) == 0 && suggestWordList.size() <= k) {
            suggestWordList.push_back(word);
        }
    }

    return suggestWordList;
}

//REMOVE A WORD FROM TRIE

bool isLeaf(Node* node) {
    if (node == nullptr)
        return false;

    for (int i = 0; i < 26; i++)
        if (node->child[i] != nullptr)
            return false;
    
    return true;
}

bool removeWord(Node*& root, std::string word, int index) {
    if (word.empty())
        return false;

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

//DELETE TRIE
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
        std::cout << "[4] Exit\n";

        int option = getch();

        if (option == '1') {

            system("cls");

            std::cout << "Choose algorithm\n";
            std::cout << "[1] Trie\n";
            std::cout << "[2] Brute-force\n";
            
            char algorithm = 0;

            while (algorithm != '1' && algorithm != '2') 
                algorithm = getch();

            system("cls");

            int k = 0;
            std::cout << "Enter the number of words you want suggested: ";
            std::cin >> k;

            system("cls");
            std::cout << "search: ";

            char curChar;
            std::string curWord;
            std::vector<std::string> suggestList;

            while (true) {
                
                //Enter a word
                curChar = getch();
                if (curChar == 8 && !curWord.empty())
                    curWord.pop_back();
                else
                    if (curChar >= 'a' && curChar <= 'z')
                        curWord.push_back(curChar);
                else if (curChar == 27)
                    break;

                system("cls");

                std::cout << "search: " << curWord << '\n';

                auto start = std::chrono::high_resolution_clock::now();

                if (algorithm ==  '1')
                    suggestList = autocompleteWord(trie, curWord, k);
                if (algorithm == '2')
                    suggestList = autocompleteWord("words_alpha.txt", curWord, k);
        
                if (suggestList.empty())
                    if (!curWord.empty())
                        std::cout << "Not found\n";

                for (auto i : suggestList) {
                    std:: cout << i << '\n';
                }

                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> runTime = end - start;

                std::cout << "Running Time: " << runTime.count() * 1000 << '\n';

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
        else if (option == '4') {
            break;
        }
    }

    destroyTrie(trie);
}

int main() {
    
    start();
    return 0;
}