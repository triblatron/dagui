//
// Created by tony on 06/08/24.
//

#include "config/config.h"

#include "util/CompletionTrie.h"
#include "util/TrieNode.h"

namespace dagui
{
    void CompletionTrie::addWord(std::string word)
    {
        _root.addWord(word);
        ++_numWords;
    }

    size_t CompletionTrie::numWords()
    {
        return _numWords;
    }

    void CompletionTrie::search(std::string substring, std::vector<std::string> &matches)
    {
        _root.search(substring,matches);
    }

    CompletionTrie::CompletionTrie()
    {
        // Do nothing.
    }
}