//
// Created by tony on 06/08/24.
//

#pragma once

#include "config/Export.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace nfe
{
    class NFE_API TrieNode
    {
    public:
        void addWord(const std::string& word)
        {
            if (word.empty())
            {
                _children.insert(ChildMap::value_type ('*',nullptr));
                return;
            }
            std::string first = word.substr(0,1);
            std::string rest;
            if (word.length()>1)
            {
                rest = word.substr(1);
            }
            if (_children.find(first[0])==_children.end())
            {
                this->_children.insert(ChildMap::value_type(first[0],new TrieNode()));
            }
            this->_children[first[0]]->addWord(rest);
        }

        void search(std::string word, std::vector<std::string>& matches)
        {
            search(std::move(word), matches, "");
        }

        void search(std::string word, std::vector<std::string>& matches, const std::string& partialMatch)
        {
                if (word.empty())
                {
                    // Depth-first search to build up possible rest of matches
                    for (auto p : _children)
                    {
                        if (p.first != '*')
                        {
                            p.second->search(word, matches, partialMatch + p.first);
                        }
                        else
                        {
                            matches.push_back(partialMatch);
                        }
                    }
                    return;
                }
                char first = word[0];

                if (auto it=_children.find(first); it!=_children.end())
                {
                    std::string rest = word.substr(1);

                    it->second->search(rest, matches, partialMatch + it->first);
                }
                else
                {
                    for (auto p : _children)
                    {
                        TrieNode * link = p.second;
                        if (p.first != '*')
                        {
                            link->search(word, matches, partialMatch + p.first);
                        }
                    }
                }
            }
    private:
        using ChildMap = std::unordered_map<char, TrieNode*>;
        ChildMap _children;

    };
}