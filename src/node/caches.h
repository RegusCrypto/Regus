// Copyright (c) 2021 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REGUS_NODE_CACHES_H
#define REGUS_NODE_CACHES_H

#include <cstddef>
#include <cstdint>

class ArgsManager;

namespace node {
struct CacheSizes {
    int64_t block_tree_db;
    int64_t coins_db;
    int64_t coins;
    int64_t tx_index;
    int64_t filter_index;
};
CacheSizes CalculateCacheSizes(const ArgsManager& args, size_t n_indexes = 0);
} // namespace node

#endif // REGUS_NODE_CACHES_H
