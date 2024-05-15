// Copyright (c) 2018-2022 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>

#include <bech32.h>
#include <util/strencodings.h>

#include <string>
#include <vector>


static void Bech32Encode(benchmark::Bench& bench)
{
    std::vector<uint8_t> v = ParseHex("c97f5a67ec381b760aeaf67573bc164845ff39a3bb26a1cee401ac67243b48db");
    std::vector<unsigned char> tmp = {0};
    tmp.reserve(1 + 32 * 8 / 5);
    ConvertBits<8, 5, true>([&](unsigned char c) { tmp.push_back(c); }, v.begin(), v.end());
    bench.batch(v.size()).unit("byte").run([&] {
        bech32::Encode(bech32::Encoding::BECH32, "bc", tmp);
    });
}


static void Bech32Decode(benchmark::Bench& bench)
{
    std::string addr = "rgs1qe9l45elv8qdhvzh27e6h80qkfpzl7wdrhvn2rnhyqxkxwfpmfrdsk88rpv";
    bench.batch(addr.size()).unit("byte").run([&] {
        bech32::Decode(addr);
    });
}


BENCHMARK(Bech32Encode, benchmark::PriorityLevel::HIGH);
BENCHMARK(Bech32Decode, benchmark::PriorityLevel::HIGH);
