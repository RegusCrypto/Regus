// Copyright (c) 2011-2020 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REGUS_QT_REGUSADDRESSVALIDATOR_H
#define REGUS_QT_REGUSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class RegusAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RegusAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Regus address widget validator, checks for a valid regus address.
 */
class RegusAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RegusAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // REGUS_QT_REGUSADDRESSVALIDATOR_H
