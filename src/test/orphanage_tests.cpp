// Copyright (c) 2011-2022 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <arith_uint256.h>
#include <primitives/transaction.h>
#include <pubkey.h>
#include <script/sign.h>
#include <script/signingprovider.h>
#include <test/util/random.h>
#include <test/util/setup_common.h>
#include <txorphanage.h>

#include <array>
#include <cstdint>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(orphanage_tests, TestingSetup)

class TxOrphanageTest : public TxOrphanage
{
public:
    inline size_t CountOrphans() const EXCLUSIVE_LOCKS_REQUIRED(!m_mutex)
    {
        LOCK(m_mutex);
        return m_orphans.size();
    }

    CTransactionRef RandomOrphan() EXCLUSIVE_LOCKS_REQUIRED(!m_mutex)
    {
        LOCK(m_mutex);
        std::map<Txid, OrphanTx>::iterator it;
        it = m_orphans.lower_bound(Txid::FromUint256(InsecureRand256()));
        if (it == m_orphans.end())
            it = m_orphans.begin();
        return it->second.tx;
    }
};

static void MakeNewKeyWithFastRandomContext(CKey& key)
{
    std::vector<unsigned char> keydata;
    keydata = g_insecure_rand_ctx.randbytes(32);
    key.Set(keydata.data(), keydata.data() + keydata.size(), /*fCompressedIn=*/true);
    assert(key.IsValid());
}

BOOST_AUTO_TEST_CASE(DoS_mapOrphans)
{
    // This test had non-deterministic coverage due to
    // randomly selected seeds.
    // This seed is chosen so that all branches of the function
    // ecdsa_signature_parse_der_lax are executed during this test.
    // Specifically branches that run only when an ECDSA
    // signature's R and S values have leading zeros.
    g_insecure_rand_ctx = FastRandomContext{uint256{33}};

    TxOrphanageTest orphanage;
    CKey key;
    MakeNewKeyWithFastRandomContext(key);
    FillableSigningProvider keystore;
    BOOST_CHECK(keystore.AddKey(key));

    // 50 orphan transactions:
    for (int i = 0; i < 50; i++)
    {
        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = Txid::FromUint256(InsecureRand256());
        tx.vin[0].scriptSig << OP_1;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(PKHash(key.GetPubKey()));

        orphanage.AddTx(MakeTransactionRef(tx), i);
    }

    // ... and 50 that depend on other orphans:
    for (int i = 0; i < 50; i++)
    {
        CTransactionRef txPrev = orphanage.RandomOrphan();

        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = txPrev->GetHash();
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(PKHash(key.GetPubKey()));
        SignatureData empty;
        BOOST_CHECK(SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL, empty));

        orphanage.AddTx(MakeTransactionRef(tx), i);
    }

    // This really-big orphan should be ignored:
    for (int i = 0; i < 10; i++)
    {
        CTransactionRef txPrev = orphanage.RandomOrphan();

        CMutableTransaction tx;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(PKHash(key.GetPubKey()));
        tx.vin.resize(2777);
        for (unsigned int j = 0; j < tx.vin.size(); j++)
        {
            tx.vin[j].prevout.n = j;
            tx.vin[j].prevout.hash = txPrev->GetHash();
        }
        SignatureData empty;
        BOOST_CHECK(SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL, empty));
        // Reuse same signature for other inputs
        // (they don't have to be valid for this test)
        for (unsigned int j = 1; j < tx.vin.size(); j++)
            tx.vin[j].scriptSig = tx.vin[0].scriptSig;

        BOOST_CHECK(!orphanage.AddTx(MakeTransactionRef(tx), i));
    }

    // Test EraseOrphansFor:
    for (NodeId i = 0; i < 3; i++)
    {
        size_t sizeBefore = orphanage.CountOrphans();
        orphanage.EraseForPeer(i);
        BOOST_CHECK(orphanage.CountOrphans() < sizeBefore);
    }

    // Test LimitOrphanTxSize() function:
    FastRandomContext rng{/*fDeterministic=*/true};
    orphanage.LimitOrphans(40, rng);
    BOOST_CHECK(orphanage.CountOrphans() <= 40);
    orphanage.LimitOrphans(10, rng);
    BOOST_CHECK(orphanage.CountOrphans() <= 10);
    orphanage.LimitOrphans(0, rng);
    BOOST_CHECK(orphanage.CountOrphans() == 0);
}

BOOST_AUTO_TEST_SUITE_END()