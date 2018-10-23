/**
 * Copyright 2018-present Antonio Mallia <me@antoniomallia.it>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "benchmark/benchmark.h"
#include "simple/simple9.hpp"
#include "test/util.hpp"

static void encode(benchmark::State &state) {

    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<uint32_t> values = generate_random_vector(state.range(0), (1 << 28) - 1);
        state.ResumeTiming();
        std::vector<uint8_t> buf(4 * values.size());
        simple9::encode(buf.data(), values.data(), values.size());
    }
}
BENCHMARK(encode)->Range(1 << 10, 1 << 20);

static void decode(benchmark::State &state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<uint32_t> values = generate_random_vector(state.range(0), (1 << 28) - 1);
        std::vector<uint8_t>  buf(4 * values.size());
        simple9::encode(buf.data(), values.data(), values.size());
        state.ResumeTiming();
        std::vector<uint32_t> decoded_values(values.size());
        simple9::decode(decoded_values.data(), buf.data(), values.size());
    }
}
BENCHMARK(decode)->Range(1<<10, 1<<20);

BENCHMARK_MAIN();

// 2018-10-23 04:31:23
// Running ./bench/simple9_bench
// Run on (4 X 2400 MHz CPU s)
// CPU Caches:
//   L1 Data 32K (x2)
//   L1 Instruction 32K (x2)
//   L2 Unified 262K (x2)
//   L3 Unified 3145K (x1)
// ------------------------------------------------------
// Benchmark               Time           CPU Iterations
// ------------------------------------------------------
// encode/1024        158961 ns     158309 ns       4398
// encode/4096        622553 ns     618542 ns       1029
// encode/32768      4061754 ns    4043080 ns        174
// encode/262144    33352482 ns   33061571 ns         21
// encode/1048576  134577287 ns  133459200 ns          5
// decode/1024         42589 ns      42302 ns      16247
// decode/4096        195650 ns     189514 ns       4485
// decode/32768      1063842 ns    1059042 ns        666
// decode/262144     8824501 ns    8795423 ns         78
// decode/1048576   34740134 ns   34580100 ns         20
