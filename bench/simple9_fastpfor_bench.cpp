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
#include "FastPFor/headers/simple9.h"
#include "test/util.hpp"

static void encode(benchmark::State &state) {

    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<uint32_t> values = generate_random_vector(state.range(0), (1 << 28)-1);
        state.ResumeTiming();
        FastPForLib::Simple9<false> codec;
        std::vector<uint8_t> buf(4 * values.size());
        size_t out_len = buf.size();
        codec.encodeArray(values.data(), values.size(), reinterpret_cast<uint32_t *>(buf.data()), out_len);
    }
}
BENCHMARK(encode)->Range(1 << 10, 1 << 20);

static void decode(benchmark::State &state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<uint32_t> values = generate_random_vector(state.range(0), (1 << 28)-1);
        std::vector<uint8_t>  buf(4 * values.size());
        FastPForLib::Simple9<false> codec;
        size_t out_len = buf.size();
        codec.encodeArray(values.data(), values.size(), reinterpret_cast<uint32_t *>(buf.data()), out_len);
        state.ResumeTiming();
        std::vector<uint32_t> decoded_values(values.size()*2);
        auto n = values.size();
        codec.decodeArray(reinterpret_cast<uint32_t const *>(buf.data()), buf.size(), decoded_values.data(), n);
    }
}
BENCHMARK(decode)->Range(1<<10, 1<<20);

BENCHMARK_MAIN();

// 2018-10-23 03:39:30
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
// encode/1024        167884 ns     165088 ns       4458
// encode/4096        638735 ns     629346 ns       1075
// encode/32768      4034489 ns    4019283 ns        173
// encode/262144    31953749 ns   31899955 ns         22
// encode/1048576  131967822 ns  131307333 ns          6
// decode/1024         42238 ns      41892 ns      15619
// decode/4096        186026 ns     183322 ns       4060
// decode/32768      1802515 ns    1460434 ns        603
// decode/262144     9763825 ns    9525571 ns         56
// decode/1048576   41611408 ns   39585900 ns         20
