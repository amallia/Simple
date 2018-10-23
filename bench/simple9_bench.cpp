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
        std::vector<uint32_t> values = generate_random_vector(state.range(0), 1 << 28);
        state.ResumeTiming();
        std::vector<uint8_t> buf(4 * values.size());
        simple9::encode(buf.data(), values.data(), values.size());
    }
}
BENCHMARK(encode)->Range(1 << 10, 1 << 20);

BENCHMARK_MAIN();
