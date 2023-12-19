#include "RendererShould.hpp"
#include "AvTransportActions.hpp"
#include "ConnectionManagerActions.hpp"
#include "Descriptions.hpp"
#include "Item.hpp"
#include "MediaRenderer.hpp"
#include "ProtocolInfoResponse.hpp"
#include "Renderer.hpp"
#include "SoapCallDouble.hpp"
#include "SoapMessageTransmitterDouble.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace UPnPAV;

namespace Multimedia
{

struct AvTransportUriData
{
    quint32 instanceId = quint32{1234};
    QString uri;
    QString uriMetaData;

    friend bool operator==(AvTransportUriData const &lhs, AvTransportUriData const &rhs)
    {
        return ((lhs.instanceId == rhs.instanceId) and (lhs.uri == rhs.uri) and (lhs.uriMetaData == rhs.uriMetaData));
    }
};

struct PlayData
{
    quint32 instanceId = quint32{1234};

    friend bool operator==(PlayData const &lhs, PlayData const &rhs)
    {
        return ((lhs.instanceId == rhs.instanceId));
    }
};

class TestMediaRenderer : public UPnPAV::MediaRenderer
{
public:
    using UPnPAV::MediaRenderer::MediaRenderer;
    TestMediaRenderer(DeviceDescription desc, QSharedPointer<SoapMessageTransmitter> transmitter)
        : UPnPAV::MediaRenderer{std::move(desc), std::move(transmitter)}
    {
        mProtoInfoCall->setRawMessage(ValidProtoclInfoResponseOfRenderer);
    }

    ~TestMediaRenderer() override = default;
    Q_DISABLE_COPY_MOVE(TestMediaRenderer)

    std::unique_ptr<PendingSoapCall> protocolInfo() noexcept override
    {
        mIsProtocolInfoCalled = true;
        return std::make_unique<PendingSoapCall>(mProtoInfoCall);
    }

    QSharedPointer<SoapCallDouble> protocolInfoCall() const noexcept
    {
        return mProtoInfoCall;
    }

    bool isProtocolInfoCalled() const noexcept
    {
        return mIsProtocolInfoCalled;
    }

    std::optional<std::unique_ptr<PendingSoapCall>> setAvTransportUri(quint32 instanceId,
                                                                      QString const &uri,
                                                                      QString const &uriMetaData = QString{
                                                                          ""}) noexcept override
    {
        mIsSetAvTranstportUriCalled = true;
        mSetAvTransportUriData = AvTransportUriData{.instanceId = instanceId, .uri = uri, .uriMetaData = uriMetaData};
        return std::make_unique<PendingSoapCall>(mSetAvTransportUriCall);
    }

    bool isSetAvTransportUriCalled() const noexcept
    {
        return mIsSetAvTranstportUriCalled;
    }

    AvTransportUriData avTransportUriData() const noexcept
    {
        return mSetAvTransportUriData;
    }

    QSharedPointer<SoapCallDouble> avTransportUriCall() const noexcept
    {
        return mSetAvTransportUriCall;
    }

    std::optional<std::unique_ptr<PendingSoapCall>> play(quint32 instanceId) override
    {
        mIsPlayCalled = true;
        mPlayData = PlayData{.instanceId = instanceId};
        return std::make_unique<PendingSoapCall>(mPlayCall);
    }

    bool isPlayCalled() const noexcept
    {
        return mIsPlayCalled;
    }

    PlayData playData() const noexcept
    {
        return mPlayData;
    }

    QSharedPointer<SoapCallDouble> playCall() const noexcept
    {
        return mPlayCall;
    }

private:
    QSharedPointer<SoapCallDouble> mProtoInfoCall =
        QSharedPointer<SoapCallDouble>::create(UPnPAV::validConnectionManagerSCPD(), UPnPAV::GetProtocolInfo());
    bool mIsProtocolInfoCalled = false;
    QSharedPointer<SoapCallDouble> mSetAvTransportUriCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createSetAVTransportURIAction());
    bool mIsSetAvTranstportUriCalled = false;
    AvTransportUriData mSetAvTransportUriData;
    QSharedPointer<SoapCallDouble> mPlayCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createPlayAction());
    bool mIsPlayCalled = false;
    PlayData mPlayData;
};

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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
                                                            QSharedPointer<SoapMessageTransmitterDouble>::create());
    auto upnpRendererRaw = upnpRenderer.get();
    auto renderer = Renderer{std::move(upnpRenderer)};

    renderer.initialize();

    QCOMPARE(upnpRendererRaw->isProtocolInfoCalled(), true);
}

void RendererShould::signal_that_initialization_successful_finished()
{
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
    auto upnpRenderer = std::make_unique<TestMediaRenderer>(validRendererDeviceDescription(),
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
