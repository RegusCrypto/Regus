// Copyright (c) 2016-2022 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <bench/data.h>

#include <chainparams.h>
#include <common/args.h>
#include <consensus/validation.h>
#include <streams.h>
#include <util/chaintype.h>
#include <validation.h>

// These are the two major time-sinks which happen after we have fully received
// a block off the wire, but before we can relay the block on to peers using
// compact block relay.

static void DeserializeBlockTest(benchmark::Bench& bench)
{
    DataStream stream(benchmark::data::genesisBlock);
    std::byte a{0};
    stream.write({&a, 1}); // Prevent compaction

    bench.unit("block").run([&] {
        CBlock block;
        stream >> TX_WITH_WITNESS(block);
        bool rewound = stream.Rewind(benchmark::data::genesisBlock.size());
        assert(rewound);
    });
}

static void DeserializeAndCheckBlockTest(benchmark::Bench& bench)
{
    DataStream stream(benchmark::data::genesisBlock);
    std::byte a{0};
    stream.write({&a, 1}); // Prevent compaction

    const auto chainParams = CreateChainParams(ChainType::MAIN);

    bench.unit("block").run([&] {
        CBlock block; // Note that CBlock caches its checked state, so we need to recreate it here
        stream >> TX_WITH_WITNESS(block);
        bool rewound = stream.Rewind(benchmark::data::genesisBlock.size());
        assert(rewound);

        BlockValidationState validationState;
        bool checked = CheckBlock(block, validationState, chainParams->GetConsensus());
        assert(checked);
    });
}

BENCHMARK(DeserializeBlockTest, benchmark::PriorityLevel::HIGH);
BENCHMARK(DeserializeAndCheckBlockTest, benchmark::PriorityLevel::HIGH);
