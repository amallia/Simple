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

#include <utility>
#include <iostream>

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

static size_t pack(uint32_t *&out, const uint32_t *&in, size_t n) {
    uint32_t v = 0;
    for (size_t k = 0; k < selectors_size; ++k) {
        size_t packed = 0;
        *out = 0;
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

template <size_t bits, size_t items>
static void unpack(uint32_t *&out, const uint32_t *&in) {
    const auto mask = (1UL << bits) - 1;
    for (uint32_t i = 0; i < items; ++i) {
        *out = (in[0] >> (28 - (i+1) * bits)) & mask;
    }
    out += items;
    in += 1;
}

static size_t unpack(uint32_t *&out, const uint32_t *&in) {
    auto   k        = *(in) >> 28;
    switch (k) {
        case 0:
            unpack<selectors[0].bits, selectors[0].items>(out, in);
            break;
        case 1:
            unpack<selectors[1].bits, selectors[1].items>(out, in);
            break;
        case 2:
            unpack<selectors[2].bits, selectors[2].items>(out, in);
            break;
        case 3:
            unpack<selectors[3].bits, selectors[3].items>(out, in);
            break;
        case 4:
            unpack<selectors[4].bits, selectors[4].items>(out, in);
            break;
        case 5:
            unpack<selectors[5].bits, selectors[5].items>(out, in);
            break;
        case 6:
            unpack<selectors[6].bits, selectors[6].items>(out, in);
            break;
        case 7:
            unpack<selectors[7].bits, selectors[7].items>(out, in);
            break;
        case 8:
            unpack<selectors[8].bits, selectors[8].items>(out, in);
            break;
    }
    return selectors[k].items;
}

} // namespace details

static void encode(uint8_t *out, const uint32_t *in, size_t n) {
    uint32_t *output = reinterpret_cast<uint32_t *>(out);
    const uint32_t *input = in;
    size_t value_remaining = n;
    while (value_remaining > 0) {
        auto encoded = details::pack(output, input, value_remaining);
        value_remaining -= encoded;
    }
}

static void decode(uint32_t *out, const uint8_t *in, size_t n) {
    uint32_t *output = out;
    const uint32_t *input = reinterpret_cast<const uint32_t *>(in);

    size_t value_remaining = n;
    while (value_remaining >= 28) {
        auto decoded = details::unpack(output, input);
        value_remaining -= decoded;
    }
    while(value_remaining > 0) {
        auto   k        = *input >> 28;
        auto bits = details::selectors[k].bits;
        for (size_t i = 0; i < details::selectors[k].items && i <= value_remaining; ++i)
        {
            const uint32_t mask = (1UL << bits) - 1;
            *(output++) = (*input >> (28 - (i+1) * bits)) & mask;
            value_remaining -= 1;
        }
        input += 1;
    }
}
} // namespace simple9