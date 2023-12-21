#include "RendererShould.hpp"
#include "Descriptions.hpp"
#include "Item.hpp"
#include "MediaRendererDouble.hpp"
#include "ProtocolInfoResponse.hpp"
#include "Renderer.hpp"
#include "SoapMessageTransmitterDouble.hpp"
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
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();

    QCOMPARE(upnpRendererRaw->isProtocolInfoCalled(), true);
}

void RendererShould::signal_that_initialization_successful_finished()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
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
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
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
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());

    const auto expData = AvTransportUriData{.instanceId = quint32{0},
                                            .uri = QStringLiteral("http://127.0.0.1/1234.mp3"),
                                            .uriMetaData = QString("")};
    QCOMPARE(upnpRendererRaw->isSetAvTransportUriCalled(), true);
    QCOMPARE(upnpRendererRaw->avTransportUriData(), expData);
};

void RendererShould::call_play_on_successful_avtransporturi_request()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();

    const auto expData = PlayData{.instanceId = quint32{0}};
    QCOMPARE(upnpRendererRaw->isPlayCalled(), true);
    QCOMPARE(upnpRendererRaw->playData(), expData);
}

void RendererShould::signal_the_playback_start_on_successful_play_request()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    auto playbackStartSpy = QSignalSpy{&renderer, &Renderer::playbackStarted};
    upnpRendererRaw->protocolInfoCall()->setRawMessage(ValidProtoclInfoResponseOfRenderer);

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();
    Q_EMIT upnpRendererRaw->playCall()->finished();

    QCOMPARE(playbackStartSpy.size(), 1);
}

void RendererShould::not_call_avtransporturi_with_unsupported_items()
{
    auto upnpRenderer = std::make_unique<MediaRendererDouble>(validRendererDeviceDescription(),
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    const auto playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

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
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    const auto playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

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
                                                              QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};
    const auto playbackFailedSpy = QSignalSpy{&renderer, &Renderer::playbackFailed};

    renderer.initialize();
    Q_EMIT upnpRendererRaw->protocolInfoCall()->finished();
    renderer.playback(createPlayableMediaItem());
    Q_EMIT upnpRendererRaw->avTransportUriCall()->finished();
    upnpRendererRaw->playCall()->setErrorState(true);
    Q_EMIT upnpRendererRaw->playCall()->finished();

    QCOMPARE(playbackFailedSpy.size(), 1);
    QCOMPARE(playbackFailedSpy.at(0).at(0).toString().isEmpty(), false);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::RendererShould)
