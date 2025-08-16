//
// Created by tony on 07/08/24.
//

#include "config/config.h"

#include "util/CompletionSubstring.h"

#include <algorithm>

namespace dagui
{

    void CompletionSubstring::search(std::string substring, std::vector<std::string> &matches)
    {
        for (auto it=_words.begin(); it!=_words.end(); ++it)
        {
            if ((*it).find(substring)!=std::string::npos)
            {
                matches.emplace_back(*it);
            }
        }
    }

    void CompletionSubstring::addWord(std::string word)
    {
        _words.emplace_back(word);
    }
}