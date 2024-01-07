// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ProviderLoaderShould.hpp"
#include "DummyPluginFolder.h"
#include "ProviderLoader.hpp"
#include <QTest>

namespace Multimedia
{

ProviderLoaderShould::~ProviderLoaderShould() = default;

void ProviderLoaderShould::provider_should_load_valid_plugins()
{
    auto pLoader = ProviderLoader{};
    auto const expectedName = QStringLiteral("DummyPlugin");

    auto const &providers = pLoader.load({QString(DummyPluginFolder)});

    QVERIFY2(providers.size() == 1,
             QString{"The loaded provider size \"%1\" is not the expected one %2"}
                 .arg(QString::number(providers.size()), QString::number(1))
                 .toLocal8Bit());
    QVERIFY2(providers.at(0)->name() == expectedName,
             QString{"The loaded provider name \"%1\" is not the expected one %2"}
                 .arg(providers.at(0)->name(), expectedName)
                 .toLocal8Bit());
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::ProviderLoaderShould)
