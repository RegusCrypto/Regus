// Copyright (c) 2019-2021 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/data.h>

namespace benchmark {
namespace data {

#include <bench/data/genesisBlock.raw.h>
const std::vector<uint8_t> genesisBlock{std::begin(genesisBlock_raw), std::end(genesisBlock_raw)};

} // namespace data
} // namespace benchmark
