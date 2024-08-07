//
// Created by tony on 06/08/24.
//

#pragma once

#include "config/Export.h"

#include "util/Completion.h"
#include "util/TrieNode.h"

namespace nfe
{
    class NFE_API CompletionTrie : public Completion
    {
    public:
        CompletionTrie();

        void addWord(std::string word) override;

        size_t numWords() override;

        void search(std::string substring, std::vector<std::string>& matches) override;
    private:
        TrieNode _root;
        std::size_t _numWords{0};
    };
}