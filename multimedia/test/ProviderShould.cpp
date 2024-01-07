// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ProviderShould.hpp"
#include "Provider.hpp"
#include <QTest>

namespace Multimedia
{
namespace
{

class TestProvider : public Provider
{
    Q_OBJECT
public:
    TestProvider()
        : Provider(QStringLiteral("TestProvider"),
                   ProviderVersion{.major = quint32{1}, .minor = quint32{2}, .patch = quint32{3}})
    {
    }
};

} // namespace

ProviderShould::~ProviderShould() = default;

void ProviderShould::give_the_name_of_the_provider()
{
    auto const provider = TestProvider{};
    auto const expName = QStringLiteral("TestProvider");
    auto const name = provider.name();
    QVERIFY2(name == expName,
             QStringLiteral("The provider name %1 doesn't match the expected %2 one").arg(name, expName).toLocal8Bit());
}

void ProviderShould::give_the_version_of_the_provider()
{
    auto const provider = TestProvider{};
    auto const expVersion = ProviderVersion{.major = quint32{1}, .minor = quint32{2}, .patch = quint32{3}};
    auto const version = provider.version();

    QVERIFY2(version == expVersion,
             QStringLiteral("The provider version %1 doesn't match the expected %2 one")
                 .arg(version.toString(), expVersion.toString())
                 .toLocal8Bit());
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::ProviderShould);

#include "ProviderShould.moc"
