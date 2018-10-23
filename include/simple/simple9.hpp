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

#pragma once

#include <iostream>
#include <utility>

namespace simple9 {

namespace details {

struct selector {
    size_t items;
    size_t bits;
};

constexpr selector selectors[] = {
    {28, 1}, {14, 2}, {9, 3}, {7, 4}, {5, 5}, {4, 7}, {3, 9}, {2, 14}, {1, 28}};
constexpr auto selectors_size = sizeof selectors / sizeof selectors[0];

static void append_bits(uint32_t *out, uint32_t value, size_t bits) {
    *out = (*out << bits) | value;
}

inline bool fit(uint32_t value, size_t bits) { return value < (1UL << bits); }

inline static size_t pack(uint32_t *&out, const uint32_t *&in, size_t n) {
    uint32_t v = 0;
    for (size_t k = 0; k < selectors_size; ++k) {
        size_t packed = 0;
        *out          = 0;
        append_bits(out, k, 4);
        auto m    = selectors[k].items < n ? selectors[k].items : n;
        auto bits = selectors[k].bits;
        for (size_t i = 0; i < m && packed < n; ++i) {
            v = *(in + i);
            if (!fit(v, bits)) {
                break;
            }
            append_bits(out, v, bits);
            packed += 1;
        }
        if (packed == m) {
            append_bits(out, 0, 28 - m * bits);
            out += 1;
            in += packed;
            return packed;
        }
    }
    throw std::runtime_error(std::string("Value out of range.") + std::to_string(v));
}

inline static void unpack(uint32_t *&out, const uint32_t *&in, size_t &value_remaining) {
    const auto k    = *in >> 28;
    const auto bits = details::selectors[k].bits;
    for (size_t i = 0; i < details::selectors[k].items && i <= value_remaining; ++i) {
        const uint32_t mask = (1UL << bits) - 1;
        *(out++)         = (*in >> (28 - (i + 1) * bits)) & mask;
        value_remaining -= 1;
    }
    in += 1;
}

} // namespace details

static void encode(uint8_t *out, const uint32_t *in, size_t n) {
    uint32_t *      output          = reinterpret_cast<uint32_t *>(out);
    const uint32_t *input           = in;
    size_t          value_remaining = n;
    while (value_remaining > 0) {
        auto encoded = details::pack(output, input, value_remaining);
        value_remaining -= encoded;
    }
}

static void decode(uint32_t *out, const uint8_t *in, size_t n) {
    uint32_t *      output          = out;
    const uint32_t *input           = reinterpret_cast<const uint32_t *>(in);
    size_t          value_remaining = n;
    while (value_remaining > 0) {
        details::unpack(output, input, value_remaining);
    }
}
} // namespace simple9