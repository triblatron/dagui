//
// Created by tony on 06/08/24.
//

#include <benchmark/benchmark.h>
#include "util/CompletionTrie.h"

void BM_CompletionTrieAddWord(benchmark::State& state)
{
    for (auto _ : state)
    {
        nfe::CompletionTrie sut;

        sut.addWord("Graph|Add Node|Math|Trig");
    }
}

BENCHMARK(BM_CompletionTrieAddWord);

void BM_CompletionTrieSearch(benchmark::State& state)
{
    nfe::CompletionTrie sut;
    sut.addWord("Graph|Add Node|Math|Trig");
    std::vector<std::string> matches;
    for (auto _ : state)
    {
        sut.search("Add", matches);
    }
}

BENCHMARK(BM_CompletionTrieSearch);

BENCHMARK_MAIN();
