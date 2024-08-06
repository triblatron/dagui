//
// Created by tony on 06/08/24.
//

#include <benchmark/benchmark.h>
#include "util/CompletionTrie.h"

void BM_CompletionTrie(benchmark::State& state)
{
    for (auto _ : state)
    {
        nfe::CompletionTrie sut;

        sut.addWord("Graph|Add Node|Math|Trig");
    }
}

BENCHMARK(BM_CompletionTrie);
BENCHMARK_MAIN();
