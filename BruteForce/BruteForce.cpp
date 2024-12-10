#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>

std::vector<std::string> createDictionary(std::string fileName) {
    std::fstream dictionary(fileName, std::ios::in);

    std::vector<std::string> wordList;

    std::string word;

    while(getline(dictionary, word)) {
        wordList.push_back(word);
    }

    dictionary.close();
    return wordList;
}

std::vector<std::string> suggestWord(std::vector<std::string> wordList, std::string prefix, int k) {
    std::vector<std::string> suggestList;

    if (prefix.empty())
        return suggestList;
    
    for (auto i : wordList) 
        if (i.find(prefix) == 0 && suggestList.size() < k)
            suggestList.push_back(i);

    return suggestList;
}

void start() {
    std::vector<std::string> wordList = createDictionary("words_alpha.txt");

    while (true) {
        system("cls");
        std::cout << "[1] Search\n";
        std::cout << "[2] Delete\n";
        std::cout << "[3] Insert\n";

        int option = getch();

        if (option == '1') {

            system("cls");
            std::cout << "search: ";
            std::string curWord;
            std::vector<std::string> suggestList;
            char c;

            while (true) {

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

                suggestList = suggestWord(wordList, curWord, 5);

                if (suggestList.size() == 0) {
                    if (!curWord.empty())
                        std::cout << "Not found!\n";
                    continue;
                }

                for (auto i : suggestList) {
                    std::cout << i << '\n';
                }

                suggestList.clear();
            }
        }
        else if (option == '2') {
            system("cls");

            std::string word;
            std::cout << "Enter a word to remove: ";
            std::cin >> word;

            std::cout << "Removed Successfully. Press any key to continue";

            char x = getch();
        }
        else if (option == '3') {
            system("cls");

            std::string word;
            std::cout << "Enter a word to insert to dictionary: ";
            std::cin >> word;

            std::cout << "Inserted successfully. Press any key to continue";

            char x = getch();
        }
    }
}

int main() {
    start();
    return 0;
}