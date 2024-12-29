#include "Header.h"
#include <fstream>
#include <algorithm>

std::vector<std::string> sortDictionary(std::string fileName) {
    std::fstream dictionary(fileName, std::ios::in);
    std::vector<std::string> wordList;
    std::string word;

    while (getline(dictionary, word)) {
        if (word.size() < 4)
            continue;
        wordList.push_back(word);
    }

    std::sort(wordList.begin(), wordList.end());

    return wordList;
}

std::vector<std::string>::iterator lowerBound(std::vector<std::string>& wordList, const std::string prefix, long long& comparisonCount) {
    int left = 0;
    int right = wordList.size();

    while (++comparisonCount && left < right) {
        int mid = left + (right - left) / 2;
       
        if (++comparisonCount && wordList[mid] < prefix) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return wordList.begin() + left;
}

std::vector<std::string> autocomplete(std::vector<std::string> wordList, std::string prefix, int num, long long& comparisonCount) {
    
    std::vector<std::string> suggestList;

    if (++comparisonCount && prefix.empty())
        return suggestList;

    auto i = lowerBound(wordList, prefix, comparisonCount);

    while (++comparisonCount && i != wordList.end() && ++comparisonCount && i->find(prefix) == 0 && ++comparisonCount && suggestList.size() < num) {
        suggestList.push_back(*i);
        ++i;
    }

    return suggestList;
}