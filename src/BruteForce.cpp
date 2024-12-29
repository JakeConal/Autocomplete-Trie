#include "Header.h"
#include <fstream>

std::vector<std::string> autocomplete(std::string fileName, std::string prefix, int num, long long& comparisonCount) {
    std::fstream dictionary(fileName, std::ios::in);
    std::vector<std::string> suggestList;
    std::string word;

    if (prefix.empty())
        return suggestList;

    while (++comparisonCount && getline(dictionary, word) && ++comparisonCount && suggestList.size() <= num) {
        if (++comparisonCount && !word.empty() && ++comparisonCount && word.find(prefix) == 0 && ++comparisonCount && suggestList.size() <= num) {
            suggestList.push_back(word);
        }
    }

    return suggestList;
}