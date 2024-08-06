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
        struct Link
        {
            char key;
            TrieNode* child;

            Link(char key, TrieNode* child) {
                this->key = key;
                this->child = child;
            }
        };
    public:
        void addWord(const std::string& word)
        {
            if (word.empty())
            {
                _children.insert(ChildMap::value_type ('*',new TrieNode::Link('*', nullptr)));
                return;
            }
            std::string first = word.substr(0,1);
            std::string rest;
            if (word.length()>1) {
                rest = word.substr(1);
            }
            if (_children.find(first[0])==_children.end())
            {
                this->_children.insert(ChildMap::value_type(first[0],new Link(first[0], new TrieNode())));
            }
            this->_children[first[0]]->child->addWord(rest);
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
                            p.second->child->search(word, matches, partialMatch + p.second->key);
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

                    it->second->child->search(rest, matches, partialMatch + it->second->key);
                }
                else {
                    for (auto p : _children)
                    {
                        Link * link = p.second;
                        if (link->key != '*')
                        {
                            link->child->search(word, matches, partialMatch + link->key);
                        }
                    }
                }
            }
    private:

        using ChildMap = std::unordered_map<char, Link*>;
        ChildMap _children;

    };
}