// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kernel/chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <hash.h>
#include <kernel/messagestartchars.h>
#include <logging.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <uint256.h>
#include <util/chaintype.h>
#include <util/strencodings.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

static CBlock CreateGenesisBlock(const char* pszTimestamp, uint32_t nTime, uint64_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward, const CScript& genesisOutputScript)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 0x1f0affff << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000289ddd45aff, ver=4, hashPrevBlock=00000000000000, hashMerkleRoot=7a36e7, nTime=1712232000, nBits=0x1f0affff, nNonce=5292, vtx=1)
 *   CTransaction(hash=7a36e7, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 0004ffff0a1f01044c555468652054696d65732030342f4170722f3230323420446f6e616c64205472756d70206c6f73657320617474656d707420746f206469736d69737320636c617373696669656420646f63756d656e74732063617365)
 *     CTxOut(nValue=2000.00000000, scriptPubKey=76a9147d51faa30be52b15d43c3958a17d3f687791e05c88ac)
 *   vMerkleTree: 7a36e7
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint64_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward, const std::vector<uint8_t>& genesisOutputScriptHex)
{
    const char* pszTimestamp = "The Times 04/Apr/2024 Donald Trump loses attempt to dismiss classified documents case";
    const CScript genesisOutputScript = CScript(genesisOutputScriptHex.begin(), genesisOutputScriptHex.end());
    return CreateGenesisBlock(pszTimestamp, nTime, nNonce, nBits, nVersion, genesisReward, genesisOutputScript);
}

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        m_chain_type = ChainType::MAIN;
        consensus.nSubsidyHalvingInterval = 1051000;
        consensus.BIP16Active = true;
        consensus.BIP34Active = true;
        consensus.BIP65Active = true;
        consensus.BIP66Active = true;
        consensus.CSVActive = true;
        consensus.SegwitActive = true;
        consensus.TaprootActive = true;
        consensus.powLimit = uint256S("0x000affffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetWindow = 20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9075;
        consensus.nMinerConfirmationWindow = 10080;
        
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000001745");
        consensus.defaultAssumeValid = uint256S("0x000289ddd45aff0980b1bebe6cd99988044366b9aca81b7566c17288d8fa5783");

        consensus.genesisOutputScriptHex = {0x76,0xa9,0x14,0x7d,0x51,0xfa,0xa3,0x0b,0xe5,0x2b,0x15,0xd4,0x3c,0x39,0x58,0xa1,0x7d,0x3f,0x68,0x77,0x91,0xe0,0x5c,0x88,0xac};

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xef;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0xeb;

        nDefaultPort = 4610;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 20;
        m_assumed_chain_state_size = 4;

        genesis = CreateGenesisBlock(1712232000, 0x14ac, 0x1f0affff, 4, 2000 * COIN, consensus.genesisOutputScriptHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.hashMix);

        assert(consensus.hashGenesisBlock == uint256S("0x000289ddd45aff0980b1bebe6cd99988044366b9aca81b7566c17288d8fa5783"));
        assert(genesis.hashMerkleRoot == uint256S("0x7a36e75d620bba5d387553ab5f3e81c5043f3b7445f31702c1180ad10780670a"));
        assert(genesis.hashMix == uint256S("0x3d373924b3686d6dec306caaf2801b1e484a4b30d5a0146eeeb8c53fb07e5742"));

        vSeeds.emplace_back("dns1-main.regus.app");
        vSeeds.emplace_back("dns2-main.regus.app");
        vSeeds.emplace_back("dns3-main.regus.app");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,76);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "rgs";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                { 0, uint256S("0x000289ddd45aff0980b1bebe6cd99988044366b9aca81b7566c17288d8fa5783")}
            }
        };

        m_assumeutxo_data = {

        };

        chainTxData = ChainTxData{
            .nTime    = 1712232000,
            .nTxCount = 1,
            .dTxRate  = 0
        };
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        m_chain_type = ChainType::TESTNET;
        consensus.nSubsidyHalvingInterval = 1051000;
        consensus.BIP16Active = true;
        consensus.BIP34Active = true;
        consensus.BIP65Active = true;
        consensus.BIP66Active = true;
        consensus.CSVActive = true;
        consensus.SegwitActive = true;
        consensus.TaprootActive = true;
        consensus.powLimit = uint256S("0x000affffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetWindow = 20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7560;
        consensus.nMinerConfirmationWindow = 10080;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000001745");
        consensus.defaultAssumeValid = uint256S("0x0006f1b83fd276a196a876e8e800cd82443e03cea68cea2817c81dfa529761ac");

        consensus.genesisOutputScriptHex = {0x76,0xa9,0x14,0xdc,0x49,0xd3,0x1d,0xd2,0x18,0x0b,0xb4,0x9a,0xac,0xdf,0xa6,0xad,0x8d,0x97,0x9c,0x95,0x29,0x0e,0xf3,0x88,0xac};

        pchMessageStart[0] = 0xb4;
        pchMessageStart[1] = 0xfa;
        pchMessageStart[2] = 0xdf;
        pchMessageStart[3] = 0xa4;

        nDefaultPort = 14610;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 10;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1712232000, 0x15d4, 0x1f0affff, 4, 2000 * COIN, consensus.genesisOutputScriptHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.hashMix);

        assert(consensus.hashGenesisBlock == uint256S("0x0006f1b83fd276a196a876e8e800cd82443e03cea68cea2817c81dfa529761ac"));
        assert(genesis.hashMerkleRoot == uint256S("0x9881a4493ac79d1c00b2b8f02f889d33f06386c2bd77401efe401bea1ff5e000"));
        assert(genesis.hashMix == uint256S("0x6a66fdf92e10f55ac44d73757f8304c79c38bad7455287fa7a7e1b58235c5e31"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.emplace_back("dns1-test.regus.app");
        vSeeds.emplace_back("dns2-test.regus.app");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rgt";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x0006f1b83fd276a196a876e8e800cd82443e03cea68cea2817c81dfa529761ac")},
            }
        };

        m_assumeutxo_data = {

        };

        chainTxData = ChainTxData{
            .nTime    = 1712232000,
            .nTxCount = 1,
            .dTxRate  = 0
        };
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */
class CRegTestParams : public CChainParams
{
public:
    explicit CRegTestParams()
    {
        m_chain_type = ChainType::REGTEST;
        consensus.nSubsidyHalvingInterval = 1051;
        consensus.BIP16Active = true;
        consensus.BIP34Active = true;
        consensus.BIP65Active = true;
        consensus.BIP66Active = true;
        consensus.CSVActive = true;
        consensus.SegwitActive = true;
        consensus.TaprootActive = true;
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetWindow = 20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540;
        consensus.nMinerConfirmationWindow = 720;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000002");
        consensus.defaultAssumeValid = uint256S("0x08afdf818a237a60ee0cecc8bb168e000a41b9df6ab3000c2f0e640b56bba249");

        consensus.genesisOutputScriptHex = {0x76,0xa9,0x14,0xfc,0x8e,0x5a,0x66,0xd9,0x39,0xfa,0xa5,0x01,0xed,0xf6,0x00,0xf6,0x39,0x16,0x3e,0xd5,0x8f,0xad,0x93,0x88,0xac};

        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xdc;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xfc;

        nDefaultPort = 24610;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1712232000, 0x2, 0x207fffff, 4, 2000 * COIN, consensus.genesisOutputScriptHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.hashMix);

        assert(consensus.hashGenesisBlock == uint256S("0x08afdf818a237a60ee0cecc8bb168e000a41b9df6ab3000c2f0e640b56bba249"));
        assert(genesis.hashMerkleRoot == uint256S("0x2250811be4b9be9fb3691f445cb21003a53e61cefab09dfbeb6e3b62419203c7"));
        assert(genesis.hashMix == uint256S("0xb1e614a39fad5ed33ff0516cdcd12f2c7376220cea0cd2bd4d1bead67b3bd84f"));

        vSeeds.clear();
        vSeeds.emplace_back("dummySeed.invalid.");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rgrt";

        vFixedSeeds.clear();

        fDefaultConsistencyChecks = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0x08afdf818a237a60ee0cecc8bb168e000a41b9df6ab3000c2f0e640b56bba249")},
            }
        };

        m_assumeutxo_data = {
            {
                .height = 110,
                .hash_serialized = AssumeutxoHash{uint256S("0xe78546efddc5f8b276425c147b45cfed5a2b1e02b71c765523e1b66de5931cf9")},
                .nChainTx = 111,
                .blockhash = uint256S("0x45f36119711daa114853525f53d83f532b6a1246a92dc5454518e7d51bbe15bf")
            },
            {
                .height = 299,
                .hash_serialized = AssumeutxoHash{uint256S("0xa4bf3407ccb2cc0145c49ebba8fa91199f8a3903daf0883875941497d2493c27")},
                .nChainTx = 300,
                .blockhash = uint256S("0x6ba6b7d214c94de48eca76587d1efd3cd76191b72c5d5d4c315cb5487a43f9a7")
            },
        };

        chainTxData = ChainTxData{
            .nTime    = 1712232000,
            .nTxCount = 1,
            .dTxRate  = 04
        };
    }
};

std::unique_ptr<const CChainParams> CChainParams::Main()
{
    return std::make_unique<const CMainParams>();
}

std::unique_ptr<const CChainParams> CChainParams::TestNet()
{
    return std::make_unique<const CTestNetParams>();
}

std::unique_ptr<const CChainParams> CChainParams::RegTest()
{
    return std::make_unique<const CRegTestParams>();
}