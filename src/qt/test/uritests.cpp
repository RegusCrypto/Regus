// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/test/uritests.h>

#include <qt/guiutil.h>
#include <qt/walletmodel.h>

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?req-dontexist="));
    QVERIFY(!GUIUtil::parseRegusURI(uri, &rv));

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?dontexist="));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=0.001"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=1.001"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseRegusURI("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));

    // Commas in amounts are not allowed.
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseRegusURI(uri, &rv));

    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseRegusURI(uri, &rv));

    // There are two amount specifications. The last value wins.
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=100&amount=200&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.amount == 20000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    // The first amount value is correct. However, the second amount value is not valid. Hence, the URI is not valid.
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=100&amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseRegusURI(uri, &rv));

    // Test label containing a question mark ('?').
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=100&label=?"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("?"));

    // Escape sequences are not supported.
    uri.setUrl(QString("regus:GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5?amount=100&label=%3F"));
    QVERIFY(GUIUtil::parseRegusURI(uri, &rv));
    QVERIFY(rv.address == QString("GVLzVLVZhLe44HubAVnCEZwJVNaSCzsFM5"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("%3F"));
}
