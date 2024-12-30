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

bool isPrefix(std::string word, std::string prefix, long long& comparisonCount) {
    if (++comparisonCount && prefix.size() > word.size()) return false;
    for (int i = 0; ++comparisonCount && i < prefix.size(); ++i) {
        if (++comparisonCount && word[i] != prefix[i]) return false;
    }
    return true;
}

std::vector<std::string> autocomplete(std::vector<std::string> wordList, std::string prefix, int num, long long& comparisonCount) {
    
    std::vector<std::string> suggestList;

    if (++comparisonCount && prefix.empty())
        return suggestList;

    auto i = lowerBound(wordList, prefix, comparisonCount);

    while (++comparisonCount && i != wordList.end() && ++comparisonCount && isPrefix(*i, prefix, comparisonCount) && ++comparisonCount && suggestList.size() < num) {
        suggestList.push_back(*i);
        ++i;
    }

    return suggestList;
}