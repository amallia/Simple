# Simple
[![Build Status](https://travis-ci.org/amallia/Simple.svg?branch=master)](https://travis-ci.org/amallia/Simple)

Simple encoding family: algorithms that try to pack as many numbers as possible into one machine word to achieve fast decoding.

## Codecs

### Simple9

Divides each 32-bit word into a 4-bit selector and a 28-bit payload. The selector stores one of 9 possible values, indicating how the payload is partitioned into equal-sized bitfields (e.g., 7 4-bit values, or 9 3-bit values).

#### Reference
> Vo Ngoc Anh and Alistair Moffat. 2005. Inverted Index Compression Using Word-Aligned Binary Codes. Inf. Retr.(2005) 151-166.

#### Usage
```cpp
// encode
std::vector<uint32_t> values = {1, 2, 512, 16384, 5, 8};
std::vector<uint8_t> buf(4*values.size());
simple9::encode(buf.data(), values.data(), values.size());

// decode
std::vector<uint32_t> decoded_values(values.size());
simple9::decode(decoded_values.data(), buf.data(), values.size());
```

#### Benchmarks
```
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
```
