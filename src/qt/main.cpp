// Copyright (c) 2018-2022 The Bitcoin Core developers
// Copyright (c) 2023-2024 The Regus Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/regus.h>

#include <common/url.h>
#include <compat/compat.h>
#include <util/translation.h>

#include <QCoreApplication>

#include <functional>
#include <string>

/** Translate string to current locale using Qt. */
extern const std::function<std::string(const char*)> G_TRANSLATION_FUN = [](const char* psz) {
    return QCoreApplication::translate("regus-core", psz).toStdString();
};
UrlDecodeFn* const URL_DECODE = urlDecode;

MAIN_FUNCTION
{
    return GuiMain(argc, argv);
}
