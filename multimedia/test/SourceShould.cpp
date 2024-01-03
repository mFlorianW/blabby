// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SourceShould.hpp"
#include "TestSource.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace Multimedia::TestHelper;

namespace Multimedia
{

MediaSourceShould::MediaSourceShould() = default;
MediaSourceShould::~MediaSourceShould() = default;

void MediaSourceShould::give_the_name_of_media_source()
{
    const auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("")};
    const auto expName = QStringLiteral("MusicBox");

    QVERIFY2(mediaSource.sourceName() == expName,
             QString("The MediaSource name \"%1\" is not the expected one %2")
                 .arg(mediaSource.sourceName(), expName)
                 .toLocal8Bit());
}

void MediaSourceShould::give_a_icon_url_when_set()
{
    const auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    const auto expUrl = QStringLiteral("http://localhost/musicbox.png");

    QVERIFY2(mediaSource.iconUrl() == expUrl,
             QString("The MediaSource iconUrl \"%1\" is not the expected one %2")
                 .arg(mediaSource.iconUrl(), expUrl)
                 .toLocal8Bit());
}

void MediaSourceShould::navigate_to_previous_layer()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
    const auto path1 = QStringLiteral("1");
    mediaSource.navigateTo(path1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);
    const auto path2 = QStringLiteral("2");
    mediaSource.navigateTo(path2);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("2"));
    auto navSignalSpy = QSignalSpy{&mediaSource, &Source::navigationFinished};
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
}

void MediaSourceShould::navigate_forward_to_previous_layer()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    const auto path1 = QStringLiteral("1");
    const auto path2 = QStringLiteral("2");

    mediaSource.navigateTo(path1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);
    mediaSource.navigateTo(path2);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    auto navSignalSpy = QSignalSpy{&mediaSource, &Source::navigationFinished};
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    navSignalSpy.clear();
    mediaSource.navigateForward();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateForward();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    mediaSource.navigateTo(path1);
    mediaSource.navigateTo(path2);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::MediaSourceShould);
