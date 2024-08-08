//
// Created by tony on 07/08/24.
//

#pragma once

#include "config/Export.h"

#include "util/Completion.h"

#include <unordered_map>

namespace nfe
{
    class NFE_API CompletionSubstring : public Completion
    {
    public:
        void addWord(std::string word) override;

        size_t numWords() override
        {
            return _words.size();
        }

        void search(std::string substring, std::vector<std::string>& matches) override;
    private:
        using WordMap = std::unordered_map<std::string, std::string>;
        WordMap _words;
    };
}