//
// Created by tony on 07/08/24.
//

#include "config/config.h"

#include "util/CompletionSubstring.h"

#include <algorithm>

namespace nfe
{

    void CompletionSubstring::search(std::string substring, std::vector<std::string> &matches)
    {
        std::sort(substring.begin(), substring.end());
        for (auto it=_words.begin(); it!=_words.end(); ++it)
        {
            for (auto it2=substring.begin(); it2!=substring.end(); ++it2)
            {
                if (it->first.find(*it2)!=std::string::npos)
                {
                    matches.push_back(it->second);
                }
            }
        }
    }

    void CompletionSubstring::addWord(std::string word)
    {
        std::string sorted = word;
        std::sort(sorted.begin(), sorted.end());
        _words.insert(WordMap::value_type (sorted, word));
    }
}