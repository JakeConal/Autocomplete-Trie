#include "Header.h"
#include <fstream>

std::vector<std::string> autocomplete(std::string fileName, std::string prefix, int num) {
    std::fstream dictionary(fileName, std::ios::in);
    std::vector<std::string> suggestList;
    std::string word;

    if (prefix.empty())
        return suggestList;

    while (getline(dictionary, word)) {
        if (!word.empty() && word.find(prefix) == 0 && suggestList.size() <= num) {
            suggestList.push_back(word);
        }
    }

    return suggestList;
}