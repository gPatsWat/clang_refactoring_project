#include <iostream>
#include "benchmark/benchmark.h"

// Define another benchmark
void BM_branchless(benchmark::State& state) {
        srand(1);
        const unsigned int N = state.range(0);
        std::vector<unsigned long> v1(N), v2(N);
        std::vector<int> c1(N), c2(N);
        for(size_t i = 0;i < N;++i) {
                v1[i] = rand();
                v2[i] = rand();
                c1[i] = rand() & 0x1;
                c2[i] = !c1[i];
        }

        unsigned long* p1 = v1.data();
        unsigned long* p2 = v2.data();
        int* b1 = c1.data();
        int* b2 = c2.data();
        for(auto _ : state) {
                unsigned long a1 = 0, a2 = 0;
                for(size_t i = 0;i < N;++i) {
                        unsigned long s1[2] = {p1[i] - p2[i]};
                        unsigned long s2[2] = {p1[i] * p2[i]};
                        a1 += s1[bool(b1[i])];
                        a2 += s2[bool(b1[i])];
                }
                benchmark::DoNotOptimize(a1);
                benchmark::DoNotOptimize(a2);
                benchmark::ClobberMemory();
        }
        state.SetItemsProcessed(N*state.iterations());
}

BENCHMARK(BM_branchless)->Arg(1000000)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK_MAIN();