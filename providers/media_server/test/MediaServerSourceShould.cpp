// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaServerSourceShould.h"
#include "MediaServerDouble.h"
#include "MediaServerSource.h"
#include <QTest>

namespace Provider
{

MediaServerProviderShould::~MediaServerProviderShould() = default;

void MediaServerProviderShould::give_the_name_of_the_media_server()
{
    auto mediaServer = std::make_unique<Doubles::MediaServer>();
    auto mediaServerSource = MediaServerSource{std::move(mediaServer)};
    const auto expName = QStringLiteral("MediaServer");

    const auto name = mediaServerSource.sourceName();

    QVERIFY2(
        name == expName,
        QStringLiteral("The media server name \"%1\" is not the expected one %2").arg(name, expName).toLocal8Bit());
}

void MediaServerProviderShould::give_the_icon_of_the_media_server()
{
    auto mediaServer = std::make_unique<Doubles::MediaServer>();
    auto mediaServerSource = MediaServerSource{std::move(mediaServer)};
    const auto expIcon = QStringLiteral("http://localhost:8200/icons/sm.png");

    const auto icon = mediaServerSource.iconUrl();

    QVERIFY2(
        icon == expIcon,
        QStringLiteral("The media server icon \"%1\" is not the expected one %2").arg(icon, expIcon).toLocal8Bit());
}

} // namespace Provider

QTEST_MAIN(Provider::MediaServerProviderShould);
