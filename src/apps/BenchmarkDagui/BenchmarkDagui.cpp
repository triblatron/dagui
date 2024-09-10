//
// Created by tony on 06/08/24.
//

#include <benchmark/benchmark.h>
#include "util/CompletionTrie.h"
#include "util/CompletionSubstring.h"

void setup(dagui::Completion& sut)
{
    sut.addWord("Graph|Add Node|Math|Trig");
    sut.addWord("Graph|Add Node|Math|RelOp");
    sut.addWord("Graph|Add Node|Math|Vector");
    sut.addWord("Graph|Add Node|Math|LogicOp");
    sut.addWord("Graph|Add Node|Math|Constant");
    sut.addWord("Graph|Add Node|Geometry|Read|ID");
    sut.addWord("Graph|Add Node|Geometry|Read|Index");
    sut.addWord("Graph|Add Node|Geometry|Read|Named Attribute");
    sut.addWord("Graph|Add Node|Geometry|Read|Normal");
    sut.addWord("Graph|Add Node|Geometry|Read|Position");
    sut.addWord("Graph|Add Node|Geometry|Read|Radius");
    sut.addWord("Graph|Add Node|Geometry|Sample|Geometry Proximity");
    sut.addWord("Graph|Add Node|Geometry|Sample|Index of Nearest");
    sut.addWord("Graph|Add Node|Geometry|Sample|Raycast");
    sut.addWord("Graph|Add Node|Geometry|Sample|Sample Index");
    sut.addWord("Graph|Add Node|Geometry|Sample|Sample Nearest");
    sut.addWord("Graph|Add Node|Geometry|Write|Set ID");
    sut.addWord("Graph|Add Node|Geometry|Write|Set Position");
    sut.addWord("Graph|Add Node|Geometry|Operations|Bounding Box");
    sut.addWord("Graph|Add Node|Geometry|Operations|Convex Hull");
    sut.addWord("Graph|Add Node|Geometry|Operations|Delete Geometry");
    sut.addWord("Graph|Add Node|Geometry|Operations|Duplicate Geometry");
    sut.addWord("Graph|Add Node|Geometry|Operations|Merge by Distance");
    sut.addWord("Graph|Add Node|Geometry|Operations|Transform Geometry");
    sut.addWord("Graph|Add Node|Geometry|Operations|Separate Components");
    sut.addWord("Graph|Add Node|Geometry|Geometry To Instance");
    sut.addWord("Graph|Add Node|Geometry|Join Geometry");
}

void BM_CompletionTrieAddWord(benchmark::State& state)
{
    for (auto _ : state)
    {
        dagui::CompletionTrie sut;

        sut.addWord("Graph|Add Node|Math|Trig");
    }
}

BENCHMARK(BM_CompletionTrieAddWord);

void BM_CompletionTrieSearch(benchmark::State& state)
{
    dagui::CompletionTrie sut;
    setup(sut);
    std::vector<std::string> matches;
    for (auto _ : state)
    {
        sut.search("Add", matches);
    }
}

//BENCHMARK(BM_CompletionTrieSearch);

void BM_CompletionSubstringAddWord(benchmark::State& state)
{
    for (auto _ : state)
    {
        dagui::CompletionSubstring sut;

        sut.addWord("Graph|Add Node|Math|Trig");
    }
}

BENCHMARK(BM_CompletionSubstringAddWord);

void BM_CompletionSubstringSearch(benchmark::State& state)
{
    dagui::CompletionSubstring sut;
    setup(sut);
//    sut.sort();
    std::vector<std::string> matches;
    for (auto _ : state)
    {
        sut.search("Add", matches);
    }
}

BENCHMARK(BM_CompletionSubstringSearch);

BENCHMARK_MAIN();
