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

#include "simple/simple9.hpp"
#include "util.hpp"

using ::testing::ContainerEq;

// TEST(Simple9, single_value) {
//     std::vector<uint32_t> values = {1};
//     std::vector<uint8_t> buf(4*values.size());
//     simple9::encode(buf.data(), values.data(), values.size());
//     EXPECT_EQ(buf.size(), 4);
//     std::vector<uint32_t> decoded_values(values.size());
//     simple9::decode(decoded_values.data(), buf.data(), values.size());
//     EXPECT_EQ(decoded_values.size(), values.size());
//     EXPECT_THAT(decoded_values, ContainerEq(values));
// }

// TEST(Simple9, single_word) {
//     std::vector<uint32_t> values = {1, 1};
//     std::vector<uint8_t> buf(4*values.size());
//     simple9::encode(buf.data(), values.data(), values.size());
//     EXPECT_EQ(buf.size(), 8);
//     std::vector<uint32_t> decoded_values(values.size());
//     simple9::decode(decoded_values.data(), buf.data(), values.size());
//     EXPECT_EQ(decoded_values.size(), values.size());
//     EXPECT_THAT(decoded_values, ContainerEq(values));
// }


TEST(Simple9, fuzzy) {
    size_t n = 10;
    std::vector<uint8_t> buf(4 * n);
    std::vector<uint32_t> values = { 263289169, 262467900, 210350352, 264628099, 3593238, 128369852, 237726634, 58168611, 192619969, 126664129 };//generate_random_vector(n, 1<<28);
    simple9::encode(buf.data(), values.data(), n);
    std::vector<uint32_t> decoded_values(n);
    simple9::decode(decoded_values.data(), buf.data(), n);
    EXPECT_EQ(decoded_values.size(), values.size());
    EXPECT_THAT(decoded_values, ContainerEq(values));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
