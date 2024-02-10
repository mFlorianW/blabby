// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaPlayerShould.hpp"
#include "EventBackendDouble.hpp"
#include "MediaPlayer.hpp"
#include "MediaRendererDouble.hpp"
#include "Renderer.hpp"
#include "SoapBackendDouble.hpp"
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
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(UPnPAV::validRendererDeviceDescription(),
                                                              QSharedPointer<UPnPAV::SoapBackendDouble>::create(),
                                                              QSharedPointer<UPnPAV::Doubles::EventBackend>::create());
    mUpnpRenderer = upnpRenderer.get();
    auto renderer = std::make_shared<Renderer>(std::move(upnpRenderer));
    mPlayer = std::make_shared<MediaPlayer>();

    // Testing that the media player requests the device state on initialization
    mUpnpRenderer->setDeviceState(MediaDevice::State::Playing);
    mPlayer->setRenderer(renderer);
    QCOMPARE(mPlayer->playbackState(), MediaPlayer::PlaybackState::Playing);

    // Test that the media player requests the protocol info initialization
    QCOMPARE(mUpnpRenderer->isProtocolInfoCalled(), true);
    Q_EMIT mUpnpRenderer->protocolInfoCall()->finished();

    // Set renderer back to default state
    mUpnpRenderer->setDeviceState(MediaDevice::State::Stopped);
}

void MediaPlayerShould::start_stop_playback_on_play_request()
{
    auto playbackChangedSpy = QSignalSpy{mPlayer.get(), &MediaPlayer::playbackStateChanged};

    mPlayer->play(createPlayableMediaItem());
    QCOMPARE(mUpnpRenderer->isSetAvTransportUriCalled(), true);
    Q_EMIT mUpnpRenderer->avTransportUriCall()->finished();
    QCOMPARE(mUpnpRenderer->isPlayCalled(), true);
    Q_EMIT mUpnpRenderer->playCall()->finished();
    // Simulate the device state change to playing
    mUpnpRenderer->setDeviceState(MediaDevice::State::Playing);

    QCOMPARE(playbackChangedSpy.size(), 1);
    QCOMPARE(mPlayer->playbackState(), MediaPlayer::PlaybackState::Playing);

    playbackChangedSpy.clear();
    mPlayer->stop();
    QCOMPARE(mUpnpRenderer->isStopCalled(), true);
    Q_EMIT mUpnpRenderer->stopCall()->finished();
    // Simulate the device state change to stopped
    mUpnpRenderer->setDeviceState(MediaDevice::State::Stopped);

    QCOMPARE(mPlayer->playbackState(), MediaPlayer::PlaybackState::Stopped);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaPlayerShould)
