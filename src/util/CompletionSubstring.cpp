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
        for (auto it=_words.begin(); it!=_words.end(); ++it)
        {
            if ((*it).find(substring)!=std::string::npos)
            {
                matches.push_back(*it);
            }
        }
    }

    void CompletionSubstring::addWord(std::string word)
    {
        _words.push_back(word);
    }
}