// Copyright (c) 2023 Regus Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "logprintf.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

class RegusModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<regus::LogPrintfCheck>("regus-unterminated-logprintf");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<RegusModule>
    X("regus-module", "Adds regus checks.");

volatile int RegusModuleAnchorSource = 0;
