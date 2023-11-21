// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaSourceShould.hpp"
#include "MediaSource.hpp"
#include <QTest>

namespace Multimedia
{
namespace
{
class TestMediaSource : public MediaSource
{
public:
    TestMediaSource(QString name, QString iconUrl = QString{})
        : MediaSource{std::move(name), std::move(iconUrl)}
    {
    }
};

} // namespace

MediaSourceShould::MediaSourceShould() = default;
MediaSourceShould::~MediaSourceShould() = default;

void MediaSourceShould::give_the_name_of_media_source()
{
    const auto mediaSource = TestMediaSource{QStringLiteral("MusicBox")};
    const auto expName = QStringLiteral("MusicBox");

    QVERIFY2(mediaSource.sourceName() == expName,
             QString("The MediaSource name \"%1\" is not the expected one %2")
                 .arg(mediaSource.sourceName(), expName)
                 .toLocal8Bit());
}

void MediaSourceShould::give_a_icon_url_when_set()
{
    const auto mediaSource =
        TestMediaSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    const auto expUrl = QStringLiteral("http://localhost/musicbox.png");

    QVERIFY2(mediaSource.iconUrl() == expUrl,
             QString("The MediaSource iconUrl \"%1\" is not the expected one %2")
                 .arg(mediaSource.iconUrl(), expUrl)
                 .toLocal8Bit());
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::MediaSourceShould);
