// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RendererShould.hpp"
#include "Descriptions.hpp"
#include "EventBackendDouble.hpp"
#include "Item.hpp"
#include "MediaRendererDouble.hpp"
#include "ProtocolInfoResponse.hpp"
#include "Renderer.hpp"
#include "SoapBackendDouble.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace UPnPAV;
using namespace UPnPAV::Doubles;

namespace Multimedia
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

Item createUnplayableMediaItem()
{
    return ItemBuilder{}
        .withPlayUrl(QStringLiteral("http:://127.0.0.1/1234.mp3"))
        .withSupportedTypes({Protocol::create(QStringLiteral("http-get:*:audio/mp5000")).value_or(Protocol{})})
        .build();
}

} // namespace

RendererShould::~RendererShould() = default;

void RendererShould::request_supported_protocols_on_init()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();

    QCOMPARE(upnpRendererRaw->isProtocolInfoCalled(), true);
}

void RendererShould::signal_that_initialization_successful_finished()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto finishedSpy = QSignalSpy{&renderer, &Renderer::initializationFinished};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();

    QCOMPARE(finishedSpy.size(), 1);
}

void RendererShould::signal_that_initialization_unsuccessful_finished()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto finishedSpy = QSignalSpy{&renderer, &Renderer::initializationFailed};

    renderer.initialize();
    upnpRendererRaw->protocolInfoCall()->setErrorState(true);
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();

    QCOMPARE(finishedSpy.size(), 1);
    QCOMPARE(finishedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::call_avtransport_uri_on_playback_request()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());

    auto const expData = AvTransportUriData{.instanceId = quint32{0},
                                            .uri = QStringLiteral("http://127.0.0.1/1234.mp3"),
                                            .uriMetaData = QString("")};
    QCOMPARE(upnpRendererRaw->isSetAvTransportUriCalled(), true);
    QCOMPARE(upnpRendererRaw->avTransportUriData(), expData);
};

void RendererShould::call_play_on_successful_avtransporturi_request()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();

    auto const expData = PlayData{.instanceId = quint32{0}};
    QCOMPARE(upnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(upnpRendererRaw->playData(), expData);
}

void RendererShould::not_call_avtransporturi_with_unsupported_items()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createUnplayableMediaItem());

    QCOMPARE(upnpRendererRaw->isSetAvTransportUriCalled(), false);
    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::signal_playback_failed_on_avtransporturi_call_failed()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    upnpRendererRaw->avTransportUriCall()->setErrorState(true);
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();

    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::signal_playback_failed_on_playcall_failed()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();
    upnpRendererRaw->playCall()->setErrorState(true);
    Q_EMIT upnpRendererRaw->playCall()->finished();

    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::map_upnp_devices_states_to_renderer_device_states_data()
{
    QTest::addColumn<MediaDevice::State>("state");
    QTest::addColumn<Renderer::State>("expectedState");
    QTest::addColumn<bool>("stateChanged");

    QTest::newRow("Stopped") << MediaDevice::State::Stopped << Renderer::State::Stopped << true;
    QTest::newRow("PausedPlayback") << MediaDevice::State::PausedPlayback << Renderer::State::Paused << true;
    QTest::newRow("PausedRecording") << MediaDevice::State::PausedRecording << Renderer::State::NoMedia << false;
    QTest::newRow("Playing") << MediaDevice::State::Playing << Renderer::State::Playing << true;
    QTest::newRow("Recording") << MediaDevice::State::Recording << Renderer::State::NoMedia << false;
    QTest::newRow("Transitioning") << MediaDevice::State::Transitioning << Renderer::State::NoMedia << false;
    QTest::newRow("NoMediaPresent") << MediaDevice::State::NoMediaPresent << Renderer::State::NoMedia << false;
}

void RendererShould::map_upnp_devices_states_to_renderer_device_states()
{
    QFETCH(MediaDevice::State, state);
    QFETCH(Renderer::State, expectedState);
    QFETCH(bool, stateChanged);
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto stateChangedSpy = QSignalSpy{&renderer, &Renderer::stateChanged};

    upnpRendererRaw->setDeviceState(state);

    QCOMPARE(stateChangedSpy.isEmpty(), not stateChanged);
    QCOMPARE(renderer.state(), expectedState);
}

void RendererShould::stop_request_the_playback()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.stop();

    QCOMPARE(upnpRendererRaw->isStopCalled(), true);
    QCOMPARE(upnpRendererRaw->stopData(), {.instaneId = 0});
}

void RendererShould::send_pause_request()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    upnpRendererRaw->setPauseEnabled(true);
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.stop();

    QCOMPARE(upnpRendererRaw->isPauseCalled(), true);
    QCOMPARE(upnpRendererRaw->pauseData(), {.instaneId = 0});
}

void RendererShould::resume_the_playback_when_the_states_are_stop_and_pause()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    upnpRendererRaw->setDeviceState(MediaDevice::State::Stopped);
    renderer.resume();
    QCOMPARE(upnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(upnpRendererRaw->playData(), {.instanceId = 0});

    upnpRendererRaw->reset();
    upnpRendererRaw->setDeviceState(MediaDevice::State::Playing);
    renderer.resume();
    QCOMPARE(upnpRendererRaw->isPlayCalled(), false);

    upnpRendererRaw->reset();
    upnpRendererRaw->setDeviceState(MediaDevice::State::PausedPlayback);
    renderer.resume();
    QCOMPARE(upnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(upnpRendererRaw->playData(), {.instanceId = 0});

    upnpRendererRaw->reset();
    upnpRendererRaw->setDeviceState(MediaDevice::State::NoMediaPresent);
    renderer.resume();
    QCOMPARE(upnpRendererRaw->isPlayCalled(), false);
}

void RendererShould::request_master_volume_on_init_for_instance_id_0()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapBackendDouble>::create(),
                                                              QSharedPointer<Doubles::EventBackend>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    upnpRendererRaw->setVolumeEnabled(true);
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();

    QCOMPARE(upnpRendererRaw->isVolumeCalled(), true);
    auto const expData = VolumeData{.instanceId = 0, .channel = "Master"};
    QCOMPARE(upnpRendererRaw->volumeData(), expData);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::RendererShould)
