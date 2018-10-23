# Simple
[![Build Status](https://travis-ci.org/amallia/Simple.svg?branch=master)](https://travis-ci.org/amallia/Simple)

Simple encoding family: algorithms that try to pack as many numbers as possible into one machine word to achieve fast decoding.

## Implementations

**Simple9**: divides each 32-bit word into a 4-bit selector and a 28-bit payload. The selector stores one of9 possible values, indicating how the payload is partitioned into equal-sized bitfields (e.g., 7 4-bit values, or 9 3-bit values).

Reference: Vo Ngoc Anh and Alistair Moffat. 2005. Inverted Index Compression Using Word-Aligned Binary Codes. Inf. Retr. 8, 1 (January 2005), 151-166.