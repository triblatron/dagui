//
// Created by tony on 06/08/24.
//

#pragma once

#include "config/Export.h"

#include <string>
#include <vector>

namespace nfe
{
    class NFE_API Completion
    {
    public:
        virtual ~Completion() = default;

        virtual void addWord(std::string word) = 0;

        virtual size_t numWords() = 0;

        virtual void search(std::string substring, std::vector<std::string>& matches) = 0;
    };
}
