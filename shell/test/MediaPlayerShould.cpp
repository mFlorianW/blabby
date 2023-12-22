// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaPlayerShould.hpp"
#include "MediaPlayer.hpp"
#include "MediaRendererDouble.hpp"
#include "Renderer.hpp"
#include "SoapMessageTransmitterDouble.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace UPnPAV;
using namespace UPnPAV::Doubles;
using namespace Multimedia;

namespace Shell
{

namespace
{

Item createPlayableMediaItem()
{
    return ItemBuilder{}
        .withPlayUrl("http://127.0.0.1/1234.mp3")
        .withSupportedTypes({Protocol::create(QStringLiteral("http-get:*:audio/mpeg")).value_or(Protocol{})})
        .build();
}

} // namespace
MediaPlayerShould::~MediaPlayerShould() = default;

void MediaPlayerShould::init()
{
    auto upnpRenderer =
        std::make_unique<MediaRendererDouble>(UPnPAV::validRendererDeviceDescription(),
                                              QSharedPointer<UPnPAV::SoapMessageTransmitterDouble>::create());
    mUpnpRenderer = upnpRenderer.get();
    auto renderer = std::make_shared<Renderer>(std::move(upnpRenderer));
    mPlayer = std::make_shared<MediaPlayer>();
    mPlayer->setRenderer(renderer);

    QCOMPARE(mUpnpRenderer->isProtocolInfoCalled(), true);
    Q_EMIT mUpnpRenderer->protocolInfoCall()->finished();
}

void MediaPlayerShould::start_playback_on_play_request()
{
    auto playbackStartedSpy = QSignalSpy{mPlayer.get(), &MediaPlayer::playbackStarted};

    mPlayer->play(createPlayableMediaItem());
    QCOMPARE(mUpnpRenderer->isSetAvTransportUriCalled(), true);
    Q_EMIT mUpnpRenderer->avTransportUriCall()->finished();
    QCOMPARE(mUpnpRenderer->isPlayCalled(), true);
    Q_EMIT mUpnpRenderer->playCall()->finished();

    QCOMPARE(playbackStartedSpy.size(), 1);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaPlayerShould)
