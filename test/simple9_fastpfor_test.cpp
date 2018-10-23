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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

#include "FastPFor/headers/simple9.h"
#include "util.hpp"

using ::testing::ContainerEq;


TEST(Simple9, fuzzy) {
    size_t n = 100'000'000;
    std::vector<uint32_t> values = generate_random_vector(n, 1 << 28);
    std::vector<uint8_t>  buf(4 * values.size());
    FastPForLib::Simple9<false> codec;
    size_t out_len = buf.size();
    codec.encodeArray(values.data(), values.size(), reinterpret_cast<uint32_t *>(buf.data()), out_len);
    std::vector<uint32_t> decoded_values(values.size()*2);
    codec.decodeArray(reinterpret_cast<uint32_t const *>(buf.data()), buf.size(), decoded_values.data(), n);
    decoded_values.resize(n);
    decoded_values.shrink_to_fit();
    EXPECT_EQ(decoded_values.size(), values.size());
    EXPECT_THAT(decoded_values, ContainerEq(values));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
