// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RendererShould.hpp"
#include "Descriptions.hpp"
#include "EventBackendDouble.hpp"
#include "Item.hpp"
#include "PositionInfoResponse.hpp"
#include "Renderer.hpp"
#include "SoapBackendDouble.hpp"
#include "VolumeResponse.hpp"
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

std::unique_ptr<MediaRendererDouble> createKitchenDevice(QString const& name = QStringLiteral("Kitchen"),
                                                         QString const& address = QStringLiteral("192.168.1.42"))
{
    return std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(name,
                                                                                QStringLiteral("Denon"),
                                                                                QStringLiteral("HEOS 1"),
                                                                                QStringLiteral("uuid:kitchen"),
                                                                                address),
                                                 QSharedPointer<SoapBackendDouble>::create(),
                                                 QSharedPointer<Doubles::EventBackend>::create());
}

RememberedRenderer rememberedKitchen()
{
    return RememberedRenderer{.identity = QStringLiteral("uuid:kitchen"),
                              .name = QStringLiteral("Old Kitchen"),
                              .manufacturer = QStringLiteral("Denon"),
                              .modelName = QStringLiteral("HEOS 1"),
                              .address = QStringLiteral("192.168.1.10")};
}

constexpr auto trackUri = "http://192.168.0.3:8200/MediaItems/Harbour%20Lights.flac";

QString didl(QString const& itemElements)
{
    return QStringLiteral(R"(<DIDL-Lite xmlns:dc="http://purl.org/dc/elements/1.1/" )"
                          R"(xmlns:upnp="urn:schemas-upnp-org:metadata-1-0/upnp/" )"
                          R"(xmlns="urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/">)"
                          R"(<item id="1" parentID="0" restricted="1">%1</item></DIDL-Lite>)")
        .arg(itemElements);
}

QString fullTrackMetaData()
{
    return didl(QStringLiteral("<upnp:class>object.item.audioItem.musicTrack</upnp:class>"
                               "<dc:title>Harbour Lights</dc:title>"
                               "<dc:creator>Ferry Creator</dc:creator>"
                               "<upnp:artist>The Quiet Ferries</upnp:artist>"
                               "<upnp:albumArtURI>http://192.168.0.3:8200/AlbumArt/1.jpg</upnp:albumArtURI>"));
}

} // namespace

RendererShould::~RendererShould() = default;

std::unique_ptr<Renderer> RendererShould::createTrackedRenderer(MediaDevice::State state)
{
    mUpnpRendererRaw->setDeviceState(state);
    auto clock = std::make_unique<ClockDouble>();
    mClock = clock.get();
    auto renderer = std::make_unique<Renderer>(std::move(mUpnpRenderer), std::move(clock));
    renderer->setPositionTracked(true);
    return renderer;
}

void RendererShould::init()
{
    mUpnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                          QSharedPointer<SoapBackendDouble>::create(),
                                                          QSharedPointer<Doubles::EventBackend>::create());
    mUpnpRendererRaw = mUpnpRenderer.get();
    QCOMPARE_NE(mUpnpRenderer, nullptr);
}

void RendererShould::request_supported_protocols_on_init()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.initialize();

    QCOMPARE(mUpnpRendererRaw->isProtocolInfoCalled(), true);
}

void RendererShould::signal_that_initialization_successful_finished()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto finishedSpy = QSignalSpy{&renderer, &Renderer::initializationFinished};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();

    QCOMPARE(finishedSpy.size(), 1);
}

void RendererShould::signal_that_initialization_unsuccessful_finished()
{
    auto mUpnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                               QSharedPointer<SoapBackendDouble>::create(),
                                                               QSharedPointer<Doubles::EventBackend>::create());
    auto mUpnpRendererRaw = mUpnpRenderer.get();
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto finishedSpy = QSignalSpy{&renderer, &Renderer::initializationFailed};

    renderer.initialize();
    mUpnpRendererRaw->protocolInfoCall()->setErrorState(true);
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();

    QCOMPARE(finishedSpy.size(), 1);
    QCOMPARE(finishedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::call_avtransport_uri_on_playback_request()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());

    auto const expData = AvTransportUriData{.instanceId = quint32{0},
                                            .uri = QStringLiteral("http://127.0.0.1/1234.mp3"),
                                            .uriMetaData = QString("")};
    QCOMPARE(mUpnpRendererRaw->isSetAvTransportUriCalled(), true);
    QCOMPARE(mUpnpRendererRaw->avTransportUriData(), expData);
};

void RendererShould::call_play_on_successful_avtransporturi_request()
{
    auto mUpnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                               QSharedPointer<SoapBackendDouble>::create(),
                                                               QSharedPointer<Doubles::EventBackend>::create());
    auto mUpnpRendererRaw = mUpnpRenderer.get();
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT mUpnpRendererRaw->avTransportUriCall()->finished();

    auto const expData = PlayData{.instanceId = quint32{0}};
    QCOMPARE(mUpnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(mUpnpRendererRaw->playData(), expData);
}

void RendererShould::not_call_avtransporturi_with_unsupported_items()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createUnplayableMediaItem());

    QCOMPARE(mUpnpRendererRaw->isSetAvTransportUriCalled(), false);
    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::signal_playback_failed_on_avtransporturi_call_failed()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    mUpnpRendererRaw->avTransportUriCall()->setErrorState(true);
    Q_EMIT mUpnpRendererRaw->avTransportUriCall()->finished();

    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

void RendererShould::signal_playback_failed_on_playcall_failed()
{
    auto mUpnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                               QSharedPointer<SoapBackendDouble>::create(),
                                                               QSharedPointer<Doubles::EventBackend>::create());
    auto mUpnpRendererRaw = mUpnpRenderer.get();
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto const playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT mUpnpRendererRaw->avTransportUriCall()->finished();
    mUpnpRendererRaw->playCall()->setErrorState(true);
    Q_EMIT mUpnpRendererRaw->playCall()->finished();

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

    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto stateChangedSpy = QSignalSpy{&renderer, &Renderer::stateChanged};

    mUpnpRendererRaw->setDeviceState(state);

    QCOMPARE(stateChangedSpy.isEmpty(), not stateChanged);
    QCOMPARE(renderer.state(), expectedState);
}

void RendererShould::stop_request_the_playback()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.stop();

    QCOMPARE(mUpnpRendererRaw->isStopCalled(), true);
    QCOMPARE(mUpnpRendererRaw->stopData(), {.instaneId = 0});
}

void RendererShould::send_pause_request()
{
    mUpnpRendererRaw->setPauseEnabled(true);
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.stop();

    QCOMPARE(mUpnpRendererRaw->isPauseCalled(), true);
    QCOMPARE(mUpnpRendererRaw->pauseData(), {.instaneId = 0});
}

void RendererShould::resume_the_playback_when_the_states_are_stop_and_pause()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    mUpnpRendererRaw->setDeviceState(MediaDevice::State::Stopped);
    renderer.resume();
    QCOMPARE(mUpnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(mUpnpRendererRaw->playData(), {.instanceId = 0});

    mUpnpRendererRaw->reset();
    mUpnpRendererRaw->setDeviceState(MediaDevice::State::Playing);
    renderer.resume();
    QCOMPARE(mUpnpRendererRaw->isPlayCalled(), false);

    mUpnpRendererRaw->reset();
    mUpnpRendererRaw->setDeviceState(MediaDevice::State::PausedPlayback);
    renderer.resume();
    QCOMPARE(mUpnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(mUpnpRendererRaw->playData(), {.instanceId = 0});

    mUpnpRendererRaw->reset();
    mUpnpRendererRaw->setDeviceState(MediaDevice::State::NoMediaPresent);
    renderer.resume();
    QCOMPARE(mUpnpRendererRaw->isPlayCalled(), false);
}

void RendererShould::request_master_volume_on_init_for_instance_id_0()
{
    mUpnpRendererRaw->setVolumeEnabled(true);
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.initialize();

    QCOMPARE(mUpnpRendererRaw->isVolumeCalled(), true);
    auto const expData = VolumeData{.instanceId = 0, .channel = "Master"};
    QCOMPARE(mUpnpRendererRaw->volumeData(), expData);
}

void RendererShould::give_master_volume_and_notify_about_changes()
{
    mUpnpRendererRaw->setVolumeEnabled(true);
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto volumeChangedSpy = QSignalSpy{&renderer, &Renderer::volumeChanged};

    renderer.initialize();

    // Set a valid response for the call.
    mUpnpRendererRaw->volumeCall()->setRawMessage(UPnPAV::ValidGetVolumeResponse);
    Q_EMIT mUpnpRendererRaw->volumeCall()->finished();

    QCOMPARE(renderer.volume(), 98);
    QCOMPARE(volumeChangedSpy.size(), 1);

    // Simulate event volume update
    volumeChangedSpy.clear();
    Q_EMIT mUpnpRendererRaw->masterVolumeChanged(85);
    QCOMPARE(renderer.volume(), 85);
    QCOMPARE(volumeChangedSpy.size(), 1);
}

void RendererShould::set_volume_of_upnpav_media_renderer()
{
    mUpnpRendererRaw->setVolumeEnabled(true);
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    renderer.initialize();

    renderer.setVolume(25);

    QCOMPARE(mUpnpRendererRaw->isSetVolumeCalled(), true);
    auto expData = SetVolumeData{.instanceId = 0, .channel = "Master", .volume = 25};
    QCOMPARE(mUpnpRendererRaw->setVolumeData(), expData);
}

void RendererShould::give_the_identity_manufacturer_model_and_address_of_the_renderer()
{
    auto renderer =
        Renderer{std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(QStringLiteral("Kitchen"),
                                                                                      QStringLiteral("Denon"),
                                                                                      QStringLiteral("HEOS 1"),
                                                                                      QStringLiteral("uuid:kitchen"),
                                                                                      QStringLiteral("192.168.1.42")),
                                                       QSharedPointer<SoapBackendDouble>::create(),
                                                       QSharedPointer<Doubles::EventBackend>::create())};

    QCOMPARE(renderer.identity(), QStringLiteral("uuid:kitchen"));
    QCOMPARE(renderer.manufacturer(), QStringLiteral("Denon"));
    QCOMPARE(renderer.modelName(), QStringLiteral("HEOS 1"));
    QCOMPARE(renderer.address(), QStringLiteral("192.168.1.42"));
}

void RendererShould::be_online_when_created_for_a_device()
{
    auto renderer = Renderer{createKitchenDevice()};

    QCOMPARE(renderer.availability(), Renderer::Availability::Online);
}

void RendererShould::be_offline_with_the_remembered_details_when_created_from_a_remembered_renderer()
{
    auto renderer = Renderer{rememberedKitchen()};

    QCOMPARE(renderer.availability(), Renderer::Availability::Offline);
    QCOMPARE(renderer.remembered(), rememberedKitchen());
    QCOMPARE(renderer.identity(), QStringLiteral("uuid:kitchen"));
    QCOMPARE(renderer.name(), QStringLiteral("Old Kitchen"));
    QCOMPARE(renderer.manufacturer(), QStringLiteral("Denon"));
    QCOMPARE(renderer.modelName(), QStringLiteral("HEOS 1"));
    QCOMPARE(renderer.address(), QStringLiteral("192.168.1.10"));
}

void RendererShould::go_online_with_the_refreshed_details_of_the_device()
{
    auto renderer = Renderer{rememberedKitchen()};
    auto availabilityChangedSpy = QSignalSpy{&renderer, &Renderer::availabilityChanged};
    auto detailsChangedSpy = QSignalSpy{&renderer, &Renderer::detailsChanged};

    renderer.goOnline(createKitchenDevice(QStringLiteral("Kitchen"), QStringLiteral("192.168.1.42")));

    QCOMPARE(renderer.availability(), Renderer::Availability::Online);
    QCOMPARE(availabilityChangedSpy.size(), 1);
    QCOMPARE(detailsChangedSpy.size(), 1);
    QCOMPARE(renderer.name(), QStringLiteral("Kitchen"));
    QCOMPARE(renderer.address(), QStringLiteral("192.168.1.42"));
}

void RendererShould::go_offline_and_keep_the_last_known_details()
{
    auto renderer = Renderer{createKitchenDevice()};
    auto availabilityChangedSpy = QSignalSpy{&renderer, &Renderer::availabilityChanged};

    renderer.goOffline();
    renderer.goOffline();

    QCOMPARE(renderer.availability(), Renderer::Availability::Offline);
    QCOMPARE(availabilityChangedSpy.size(), 1);
    QCOMPARE(renderer.name(), QStringLiteral("Kitchen"));
    QCOMPARE(renderer.address(), QStringLiteral("192.168.1.42"));
}

void RendererShould::give_no_playback_state_while_offline()
{
    auto device = createKitchenDevice();
    auto* deviceRaw = device.get();
    auto renderer = Renderer{std::move(device)};
    deviceRaw->setDeviceState(MediaDevice::State::Playing);
    auto stateChangedSpy = QSignalSpy{&renderer, &Renderer::stateChanged};

    renderer.goOffline();

    QCOMPARE(renderer.state(), Renderer::State::NoMedia);
    QCOMPARE(stateChangedSpy.size(), 1);
}

void RendererShould::ignore_playback_requests_while_offline()
{
    auto renderer = Renderer{rememberedKitchen()};
    auto playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    renderer.playback(createPlayableMediaItem());
    renderer.stop();
    renderer.resume();
    renderer.setVolume(25);

    QCOMPARE(renderer.volume(), quint32{0});
    QCOMPARE(renderer.iconUrl(), QString{});
    QCOMPARE(playbackFailedSpy.size(), 1);
}

void RendererShould::initialize_again_when_an_initialized_renderer_goes_online_again()
{
    auto renderer = Renderer{createKitchenDevice()};
    renderer.initialize();
    renderer.goOffline();
    auto device = createKitchenDevice();
    auto* deviceRaw = device.get();

    renderer.goOnline(std::move(device));

    QCOMPARE(deviceRaw->isProtocolInfoCalled(), true);
}

void RendererShould::not_initialize_an_uninitialized_renderer_when_it_goes_online()
{
    auto renderer = Renderer{rememberedKitchen()};
    auto device = createKitchenDevice();
    auto* deviceRaw = device.get();

    renderer.goOnline(std::move(device));

    QCOMPARE(deviceRaw->isProtocolInfoCalled(), false);
}

void RendererShould::give_no_volume_while_offline()
{
    mUpnpRendererRaw->setVolumeEnabled(true);
    mUpnpRendererRaw->volumeCall()->setRawMessage(QString{ValidGetVolumeResponse});
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->volumeCall()->finished();
    QCOMPARE_NE(renderer.volume(), quint32{0});
    auto volumeChangedSpy = QSignalSpy{&renderer, &Renderer::volumeChanged};

    renderer.goOffline();

    QCOMPARE(renderer.volume(), quint32{0});
    QCOMPARE(volumeChangedSpy.size(), 1);
}

void RendererShould::go_offline_when_the_device_does_not_answer_a_call_data()
{
    QTest::addColumn<QString>("call");

    QTest::newRow("GetProtocolInfo") << QStringLiteral("GetProtocolInfo");
    QTest::newRow("GetVolume") << QStringLiteral("GetVolume");
    QTest::newRow("SetVolume") << QStringLiteral("SetVolume");
    QTest::newRow("SetAVTransportURI") << QStringLiteral("SetAVTransportURI");
    QTest::newRow("Play") << QStringLiteral("Play");
    QTest::newRow("Pause") << QStringLiteral("Pause");
}

void RendererShould::go_offline_when_the_device_does_not_answer_a_call()
{
    QFETCH(QString, call);
    mUpnpRendererRaw->setVolumeEnabled(true);
    mUpnpRendererRaw->setPauseEnabled(true);
    mUpnpRendererRaw->setDeviceState(MediaDevice::State::Playing);
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    auto soapCall = QSharedPointer<SoapCallDouble>{};
    if (call == QStringLiteral("GetProtocolInfo")) {
        soapCall = mUpnpRendererRaw->protocolInfoCall();
    } else if (call == QStringLiteral("GetVolume")) {
        soapCall = mUpnpRendererRaw->volumeCall();
    } else if (call == QStringLiteral("SetVolume")) {
        renderer.setVolume(25);
        soapCall = mUpnpRendererRaw->setVolumeCall();
    } else if (call == QStringLiteral("SetAVTransportURI")) {
        renderer.playback(createPlayableMediaItem());
        soapCall = mUpnpRendererRaw->avTransportUriCall();
    } else if (call == QStringLiteral("Play")) {
        renderer.playback(createPlayableMediaItem());
        Q_EMIT mUpnpRendererRaw->avTransportUriCall()->finished();
        soapCall = mUpnpRendererRaw->playCall();
    } else if (call == QStringLiteral("Pause")) {
        renderer.stop();
        soapCall = mUpnpRendererRaw->pauseCall();
    }
    QCOMPARE(renderer.availability(), Renderer::Availability::Online);
    auto availabilityChangedSpy = QSignalSpy{&renderer, &Renderer::availabilityChanged};

    soapCall->setDeviceUnreachable();
    Q_EMIT soapCall->finished();

    QTRY_COMPARE(renderer.availability(), Renderer::Availability::Offline);
    QCOMPARE(availabilityChangedSpy.size(), 1);
}

void RendererShould::stay_online_when_the_device_answers_a_call_with_an_error()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    renderer.initialize();
    Q_EMIT mUpnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    auto playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    mUpnpRendererRaw->avTransportUriCall()->setErrorState(true);
    Q_EMIT mUpnpRendererRaw->avTransportUriCall()->finished();
    QCoreApplication::processEvents();

    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(renderer.availability(), Renderer::Availability::Online);
}

void RendererShould::go_offline_when_the_event_publisher_of_the_device_is_unreachable()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    Q_EMIT mUpnpRendererRaw->unreachable();

    QTRY_COMPARE(renderer.availability(), Renderer::Availability::Offline);
}

void RendererShould::stay_online_when_a_dropped_device_was_unreachable()
{
    auto renderer = Renderer{createKitchenDevice()};
    renderer.initialize();
    auto device = createKitchenDevice();
    auto* deviceRaw = device.get();
    renderer.goOnline(std::move(device));
    auto protocolInfoCall = deviceRaw->protocolInfoCall();
    protocolInfoCall->setDeviceUnreachable();
    Q_EMIT protocolInfoCall->finished();
    renderer.goOnline(createKitchenDevice());

    QCoreApplication::processEvents();

    QCOMPARE(renderer.availability(), Renderer::Availability::Online);
}

void RendererShould::give_the_current_track_reported_by_the_device_events()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    auto currentTrackChangedSpy = QSignalSpy{&renderer, &Renderer::currentTrackChanged};

    mUpnpRendererRaw->setCurrentTrack(trackUri, fullTrackMetaData());

    QCOMPARE(currentTrackChangedSpy.size(), 1);
    QCOMPARE(renderer.currentTrack().title, QStringLiteral("Harbour Lights"));
    QCOMPARE(renderer.currentTrack().artist, QStringLiteral("The Quiet Ferries"));
    QCOMPARE(renderer.currentTrack().artworkUrl, QStringLiteral("http://192.168.0.3:8200/AlbumArt/1.jpg"));
}

void RendererShould::fall_back_for_missing_current_track_details_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<QString>("metaData");
    QTest::addColumn<QString>("expectedTitle");
    QTest::addColumn<QString>("expectedArtist");

    QTest::newRow("no title") << trackUri
                              << didl("<dc:title></dc:title><upnp:artist>The Quiet Ferries</upnp:artist>")
                              << "Harbour Lights" << "";
    QTest::newRow("no artist") << trackUri << didl("<dc:title>Tide</dc:title><dc:creator>Ferry Creator</dc:creator>")
                               << "Tide" << "Ferry Creator";
    QTest::newRow("no artist and no creator") << trackUri << didl("<dc:title>Tide</dc:title>") << "Tide" << "";
    QTest::newRow("no metadata") << trackUri << "" << "Harbour Lights" << "";
    QTest::newRow("not implemented metadata") << trackUri << "NOT_IMPLEMENTED" << "Harbour Lights" << "";
    QTest::newRow("uri with query") << "http://192.168.0.3/MediaItems/43.mp3?quality=high" << "" << "43" << "";
    QTest::newRow("uri without extension") << "http://radio.example/live/stream" << "" << "stream" << "";
    QTest::newRow("no uri and no metadata") << "" << "" << "" << "";
}

void RendererShould::fall_back_for_missing_current_track_details()
{
    QFETCH(QString, uri);
    QFETCH(QString, metaData);
    QFETCH(QString, expectedTitle);
    QFETCH(QString, expectedArtist);
    auto renderer = Renderer{std::move(mUpnpRenderer)};

    mUpnpRendererRaw->setCurrentTrack(uri, metaData);

    QCOMPARE(renderer.currentTrack().title, expectedTitle);
    QCOMPARE(renderer.currentTrack().artist, expectedArtist);
    QCOMPARE(renderer.currentTrack().artworkUrl, QString{});
}

void RendererShould::give_the_current_track_of_the_polled_position_info()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Playing);
    auto currentTrackChangedSpy = QSignalSpy{renderer.get(), &Renderer::currentTrackChanged};

    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri, fullTrackMetaData()));

    QCOMPARE(currentTrackChangedSpy.size(), 1);
    QCOMPARE(renderer->currentTrack().title, QStringLiteral("Harbour Lights"));
    QCOMPARE(renderer->currentTrack().artist, QStringLiteral("The Quiet Ferries"));
}

void RendererShould::not_notify_about_an_unchanged_current_track()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Playing);
    mUpnpRendererRaw->setCurrentTrack(trackUri, fullTrackMetaData());
    auto currentTrackChangedSpy = QSignalSpy{renderer.get(), &Renderer::currentTrackChanged};

    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri, fullTrackMetaData()));

    QCOMPARE(currentTrackChangedSpy.size(), 0);
}

void RendererShould::poll_the_position_info_every_second_while_tracked_and_playing()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Playing);
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));

    mClock->advance(std::chrono::milliseconds{999});
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
    mClock->advance(std::chrono::milliseconds{1});
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 2);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));
    mClock->advance(std::chrono::seconds{1});
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 3);
}

void RendererShould::skip_a_poll_while_the_previous_request_is_pending()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Playing);

    mClock->advance(std::chrono::seconds{1});
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));
    mClock->advance(std::chrono::seconds{1});
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 2);
}

void RendererShould::not_poll_while_not_tracked()
{
    mUpnpRendererRaw->setDeviceState(MediaDevice::State::Playing);
    auto clock = std::make_unique<ClockDouble>();
    auto* clockRaw = clock.get();
    auto renderer = Renderer{std::move(mUpnpRenderer), std::move(clock)};

    clockRaw->advance(std::chrono::seconds{3});

    QCOMPARE(renderer.isPositionTracked(), false);
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 0);
}

void RendererShould::not_poll_while_not_playing_data()
{
    QTest::addColumn<MediaDevice::State>("state");

    QTest::newRow("Paused") << MediaDevice::State::PausedPlayback;
    QTest::newRow("Stopped") << MediaDevice::State::Stopped;
    QTest::newRow("No Media") << MediaDevice::State::NoMediaPresent;
}

void RendererShould::not_poll_while_not_playing()
{
    QFETCH(MediaDevice::State, state);
    auto renderer = createTrackedRenderer(state);
    // Switching the tracking on refreshes once.
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));

    for (auto tick = 0; tick < 3; ++tick) {
        mClock->advance(std::chrono::seconds{1});
    }

    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
}

void RendererShould::refresh_the_position_info_after_a_playback_state_change()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Stopped);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));

    mUpnpRendererRaw->setDeviceState(MediaDevice::State::PausedPlayback);

    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 2);
}

void RendererShould::stop_polling_when_the_tracking_is_switched_off()
{
    auto renderer = createTrackedRenderer(MediaDevice::State::Playing);
    mUpnpRendererRaw->finishPositionInfoCall(positionInfoResponse(trackUri));

    renderer->setPositionTracked(false);
    for (auto tick = 0; tick < 3; ++tick) {
        mClock->advance(std::chrono::seconds{1});
    }

    QCOMPARE(renderer->isPositionTracked(), false);
    QCOMPARE(mUpnpRendererRaw->positionInfoCallCount(), 1);
}

void RendererShould::refresh_the_position_info_when_a_tracked_renderer_goes_online()
{
    auto renderer = Renderer{rememberedKitchen(), std::make_unique<ClockDouble>()};
    renderer.setPositionTracked(true);
    auto device = createKitchenDevice();
    auto* deviceRaw = device.get();

    renderer.goOnline(std::move(device));

    QCOMPARE(deviceRaw->positionInfoCallCount(), 1);
}

void RendererShould::give_no_current_track_while_offline()
{
    auto renderer = Renderer{std::move(mUpnpRenderer)};
    mUpnpRendererRaw->setCurrentTrack(trackUri, fullTrackMetaData());
    auto currentTrackChangedSpy = QSignalSpy{&renderer, &Renderer::currentTrackChanged};

    renderer.goOffline();

    QCOMPARE(currentTrackChangedSpy.size(), 1);
    QCOMPARE(renderer.currentTrack(), CurrentTrack{});
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::RendererShould)
