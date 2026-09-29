// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Renderer.hpp"
#include "GetPositionInfoResponse.hpp"
#include "GetProtocolInfoResponse.hpp"
#include "GetVolumeResponse.hpp"
#include "MediaServerObject.hpp"
#include "PendingSoapCall.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <QPointer>
#include <QUrl>
#include <QVariant>

using namespace UPnPAV;

namespace Multimedia
{

namespace
{
constexpr auto defaultInstanceId = quint32{0};
constexpr auto pollInterval = std::chrono::seconds{1};

RememberedRenderer rememberedOf(UPnPAV::MediaRenderer const& device)
{
    return RememberedRenderer{.identity = device.udn(),
                              .name = device.name(),
                              .manufacturer = device.manufacturer(),
                              .modelName = device.modelName(),
                              .address = device.address()};
}

/**
 * Gives the file name of the URI without its extension, e.g. "Harbour Lights" for ".../Harbour%20Lights.flac".
 */
QString titleOfUri(QString const& uri)
{
    auto title = QUrl{uri}.fileName(QUrl::FullyDecoded);
    auto const extensionStart = title.lastIndexOf(QLatin1Char{'.'});
    if (extensionStart > 0) {
        title.truncate(extensionStart);
    }
    return title;
}

CurrentTrack currentTrackOf(QString const& uri, QString metaData)
{
    auto track = CurrentTrack{};
    if (not metaData.isEmpty() and metaData != QStringLiteral("NOT_IMPLEMENTED")) {
        auto const objects = MediaServerObject::createFromDidl(metaData);
        if (not objects.isEmpty()) {
            auto const& object = objects.first();
            track.title = object.title();
            track.artist = object.artist().isEmpty() ? object.creator() : object.artist();
            track.artworkUrl = object.albumArtUrl();
        }
    }
    if (track.title.isEmpty()) {
        track.title = titleOfUri(uri);
    }
    return track;
}
} // namespace

Renderer::Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer, std::unique_ptr<UPnPAV::Clock> clock)
    : mRenderer{std::move(mediaRenderer)}
    , mClock{std::move(clock)}
{
    Q_ASSERT(mRenderer != nullptr);
    Q_ASSERT(mClock != nullptr);
    mRemembered = rememberedOf(*mRenderer);
    connect(mClock.get(), &Clock::wokeUp, this, &Renderer::onClockWokeUp);
    connectDevice();
    updateCurrentTrack(mRenderer->currentTrackUri(), mRenderer->currentTrackMetaData());
    setState(mRenderer->state());
}

Renderer::Renderer(RememberedRenderer remembered, std::unique_ptr<UPnPAV::Clock> clock)
    : mRemembered{std::move(remembered)}
    , mClock{std::move(clock)}
{
    Q_ASSERT(mClock != nullptr);
    connect(mClock.get(), &Clock::wokeUp, this, &Renderer::onClockWokeUp);
}

Renderer::~Renderer() = default;

QString const& Renderer::name() const noexcept
{
    return mRemembered.name;
}

QString Renderer::iconUrl() const noexcept
{
    if (mRenderer == nullptr) {
        return {};
    }
    return mRenderer->iconUrl().toString();
}

QString const& Renderer::identity() const noexcept
{
    return mRemembered.identity;
}

QString const& Renderer::manufacturer() const noexcept
{
    return mRemembered.manufacturer;
}

QString const& Renderer::modelName() const noexcept
{
    return mRemembered.modelName;
}

QString const& Renderer::address() const noexcept
{
    return mRemembered.address;
}

RememberedRenderer const& Renderer::remembered() const noexcept
{
    return mRemembered;
}

Renderer::Availability Renderer::availability() const noexcept
{
    return mRenderer != nullptr ? Availability::Online : Availability::Offline;
}

void Renderer::goOnline(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer)
{
    Q_ASSERT(mediaRenderer != nullptr);
    Q_ASSERT(mediaRenderer->udn() == mRemembered.identity);

    auto const wasOffline = availability() == Availability::Offline;
    auto const remembered = rememberedOf(*mediaRenderer);
    dropDevice();
    mRenderer = std::move(mediaRenderer);
    connectDevice();
    updateCurrentTrack(mRenderer->currentTrackUri(), mRenderer->currentTrackMetaData());
    setState(mRenderer->state());
    if (mInitialized) {
        initialize();
    }
    if (mPositionTracked) {
        requestPositionInfo();
    }
    updatePolling();

    if (wasOffline) {
        Q_EMIT availabilityChanged();
    }
    if (mRemembered != remembered) {
        mRemembered = remembered;
        Q_EMIT detailsChanged();
    }
}

void Renderer::goOffline() noexcept
{
    if (mRenderer == nullptr) {
        return;
    }

    dropDevice();
    updatePolling();
    updateVolume(0);
    updateCurrentTrack(QString{}, QString{});
    if (mState != State::NoMedia) {
        mState = State::NoMedia;
        Q_EMIT stateChanged();
    }

    // Emitted last, the receivers may react on the Offline Renderer right away, e.g. by disconnecting it.
    Q_EMIT availabilityChanged();
}

void Renderer::dropDevice() noexcept
{
    // The pending calls are dropped before the device they belong to.
    mProtoInfoCall.reset();
    mSetAvTransportUriCall.reset();
    mPlayCall.reset();
    mStopCall.reset();
    mResumeCall.reset();
    mVolumeCall.reset();
    mSetVolumeCall.reset();
    mPositionInfoCall.reset();
    mPositionInfoPending = false;
    mRenderer.reset();
    mProtocols.clear();
}

void Renderer::connectDevice() noexcept
{
    connect(mRenderer.get(), &MediaRenderer::stateChanged, this, [this]() {
        setState(mRenderer->state());
    });

    connect(mRenderer.get(), &MediaRenderer::masterVolumeChanged, this, [this](quint32 volume) {
        updateVolume(volume);
    });

    connect(mRenderer.get(), &MediaRenderer::currentTrackChanged, this, [this]() {
        updateCurrentTrack(mRenderer->currentTrackUri(), mRenderer->currentTrackMetaData());
    });

    connect(mRenderer.get(), &MediaRenderer::unreachable, this, &Renderer::onDeviceUnreachable);
}

std::unique_ptr<UPnPAV::PendingSoapCall> Renderer::goOfflineWhenUnreachable(
    std::unique_ptr<UPnPAV::PendingSoapCall> call) noexcept
{
    connect(call.get(), &UPnPAV::PendingSoapCall::finished, this, [this, callPtr = call.get()]() {
        if (callPtr->errorCode() == PendingSoapCall::ErrorCode::DeviceUnreachable) {
            onDeviceUnreachable();
        }
    });
    return call;
}

void Renderer::onDeviceUnreachable() noexcept
{
    // Going Offline drops the device and its pending calls, which must not happen while one of them is emitting.
    // The device may also be replaced in the meantime, then the Renderer is Online with the new device.
    QMetaObject::invokeMethod(
        this,
        [this, device = QPointer<MediaRenderer>{mRenderer.get()}]() {
            if (device.isNull() or device.get() != mRenderer.get()) {
                return;
            }
            qCWarning(mmRenderer) << "Renderer" << mRemembered.name << "is Offline. Error: the device doesn't answer";
            goOffline();
        },
        Qt::QueuedConnection);
}

void Renderer::initialize() noexcept
{
    if (mRenderer == nullptr) {
        qCWarning(mmRenderer) << "Failed to initialize Renderer" << mRemembered.name << "Error: it is Offline";
        return;
    }

    mInitialized = true;

    mProtoInfoCall = goOfflineWhenUnreachable(mRenderer->protocolInfo());
    connect(mProtoInfoCall.get(), &UPnPAV::PendingSoapCall::finished, this, [this]() {
        if (mProtoInfoCall->hasError()) {
            Q_EMIT initializationFailed(QString{"Failed to request the supported protocols. Error: %1"}.arg(
                QVariant::fromValue<UPnPAV::PendingSoapCall::ErrorCode>(mProtoInfoCall->errorCode()).toString()));
            return;
        }

        auto const protoInfoResponse = mProtoInfoCall->resultAs<GetProtocolInfoResponse>();
        mProtocols = protoInfoResponse->sinkProtocols();

        Q_EMIT initializationFinished();
    });

    auto call = mRenderer->volume(0, "Master");
    if (call.has_value()) {
        mVolumeCall = goOfflineWhenUnreachable(std::move(call.value()));
        connect(mVolumeCall.get(), &UPnPAV::PendingSoapCall::finished, this, [this]() {
            if (mVolumeCall->hasError()) {
                qCCritical(mmRenderer) << "Failed to request the \"Master\" channel volume for instance id 0. Error:"
                                       << mVolumeCall->errorDescription();
                return;
            }
            updateVolume(mVolumeCall->resultAs<UPnPAV::GetVolumeResponse>()->volume());
        });
    }
}

void Renderer::playback(Item const& item) noexcept
{
    if (mRenderer == nullptr) {
        Q_EMIT playbackFailed(QStringLiteral("The Renderer is Offline."));
        return;
    }

    if (not isPlayableItem(item)) {
        Q_EMIT playbackFailed(QString("Unsupported item passed. Now fitting protocol found."));
        return;
    }

    auto uriCall = mRenderer->setAvTransportUri(quint32{0}, item.playUrl());
    if (uriCall.has_value()) {
        mSetAvTransportUriCall = goOfflineWhenUnreachable(std::move(uriCall.value()));
        connect(mSetAvTransportUriCall.get(),
                &UPnPAV::PendingSoapCall::finished,
                this,
                &Renderer::onSetAvTransportUriFinished);
    }
}

bool Renderer::isPlayableItem(Item const& item) const noexcept
{
    for (auto const& protocol : mProtocols) {
        for (auto const& itemProto : item.supportedTypes()) {
            if (itemProto == protocol) {
                return true;
            }
        }
    }

    return false;
}

void Renderer::onSetAvTransportUriFinished() noexcept
{
    if (mSetAvTransportUriCall and not mSetAvTransportUriCall->hasError()) {
        auto playCall = mRenderer->play(defaultInstanceId);
        if (playCall.has_value()) {
            mPlayCall = goOfflineWhenUnreachable(std::move(playCall.value()));
            connect(mPlayCall.get(), &UPnPAV::PendingSoapCall::finished, this, &Renderer::onPlayCallFinished);
        }
    } else if (mSetAvTransportUriCall and mSetAvTransportUriCall->hasError()) {
        Q_EMIT playbackFailed(QString{"Failed to set AvTransport URI. Error code: %1. Error description: %2"}.arg(
            QVariant::fromValue<PendingSoapCall::ErrorCode>(mSetAvTransportUriCall->errorCode()).toString(),
            mSetAvTransportUriCall->errorDescription()));
    }
}

void Renderer::onPlayCallFinished() noexcept
{
    if (mPlayCall and mPlayCall->hasError()) {
        Q_EMIT playbackFailed(QString{"Failed to call play. Error code: %1. Error description: %2"}.arg(
            QVariant::fromValue<PendingSoapCall::ErrorCode>(mPlayCall->errorCode()).toString(),
            mPlayCall->errorDescription()));
    }
}

void Renderer::stop() noexcept
{
    if (mRenderer == nullptr) {
        qCWarning(mmRenderer) << "Stop call is not possible. Error: Renderer is Offline";
        return;
    }

    if (not mRenderer->hasAvTransportService()) {
        qCCritical(mmRenderer) << "Stop call is not possible. Error: Renderer doesn't have AvTransport service";
        return;
    }

    auto stopCall = mRenderer->pause(0);
    if (not stopCall.has_value()) {
        stopCall = mRenderer->stop(0);
    }

    if (stopCall.has_value()) {
        mStopCall = goOfflineWhenUnreachable(std::move(stopCall.value()));
        connect(mStopCall.get(), &UPnPAV::PendingSoapCall::finished, this, [this]() {
            if (mStopCall->hasError()) {
                qCCritical(mmRenderer) << "Stop request failed with error:" << mStopCall->errorDescription();
            }
        });
    }
}

void Renderer::resume() noexcept
{
    if (mRenderer != nullptr and (mState == State::Stopped or mState == State::Paused)) {
        auto resumeCall = mRenderer->play(0);
        if (resumeCall.has_value()) {
            mResumeCall = goOfflineWhenUnreachable(std::move(resumeCall.value()));
        }
    }
}

Renderer::State Renderer::state() const noexcept
{
    return mState;
}

quint32 Renderer::volume() const noexcept
{
    return mVolume;
}

void Renderer::setVolume(quint32 volume) noexcept
{
    if (mRenderer == nullptr) {
        qCWarning(mmRenderer) << "Failed to set volume. Error: Renderer is Offline";
        return;
    }

    auto call = mRenderer->setVolume(0, "Master", volume);
    if (call.has_value()) {
        mSetVolumeCall = goOfflineWhenUnreachable(std::move(call.value()));
        connect(mSetVolumeCall.get(), &UPnPAV::PendingSoapCall::finished, this, [this] {
            if (mSetVolumeCall->hasError()) {
                qCCritical(mmRenderer) << "Failed to set set volume. Error:" << mSetVolumeCall->errorDescription();
            }
        });
    }
}

void Renderer::setState(UPnPAV::MediaRenderer::State state) noexcept
{
    auto newState = State::NoMedia;
    if (state == MediaRenderer::State::Playing) {
        newState = State::Playing;
    } else if (state == MediaRenderer::State::PausedPlayback) {
        newState = State::Paused;
    } else if (state == MediaRenderer::State::Stopped) {
        newState = State::Stopped;
    }

    if (mState != newState) {
        mState = newState;
        Q_EMIT stateChanged();
        if (mPositionTracked) {
            requestPositionInfo();
        }
        updatePolling();
    }
}

void Renderer::updateVolume(quint32 volume) noexcept
{
    if (mVolume != volume) {
        mVolume = volume;
        Q_EMIT volumeChanged();
        qCDebug(mmRenderer) << "Volume received:" << mVolume;
    }
}

CurrentTrack const& Renderer::currentTrack() const noexcept
{
    return mCurrentTrack;
}

void Renderer::setPositionTracked(bool tracked) noexcept
{
    if (mPositionTracked == tracked) {
        return;
    }

    mPositionTracked = tracked;
    if (mPositionTracked) {
        requestPositionInfo();
    }
    updatePolling();
}

bool Renderer::isPositionTracked() const noexcept
{
    return mPositionTracked;
}

void Renderer::updateCurrentTrack(QString const& uri, QString const& metaData) noexcept
{
    if (mCurrentTrackUri == uri and mCurrentTrackMetaData == metaData) {
        return;
    }

    mCurrentTrackUri = uri;
    mCurrentTrackMetaData = metaData;
    setCurrentTrack(currentTrackOf(uri, metaData));
}

void Renderer::setCurrentTrack(CurrentTrack const& track) noexcept
{
    if (mCurrentTrack != track) {
        mCurrentTrack = track;
        Q_EMIT currentTrackChanged();
    }
}

void Renderer::requestPositionInfo() noexcept
{
    if (mRenderer == nullptr or mPositionInfoPending) {
        return;
    }

    auto call = mRenderer->positionInfo(defaultInstanceId);
    if (not call.has_value()) {
        return;
    }
    mPositionInfoPending = true;
    mPositionInfoCall = goOfflineWhenUnreachable(std::move(call.value()));
    connect(mPositionInfoCall.get(), &PendingSoapCall::finished, this, &Renderer::onPositionInfoFinished);
}

void Renderer::onPositionInfoFinished() noexcept
{
    mPositionInfoPending = false;
    if (mPositionInfoCall->hasError()) {
        qCWarning(mmRenderer) << "Failed to request the position info of" << mRemembered.name
                              << "Error:" << mPositionInfoCall->errorDescription();
        return;
    }

    auto const response = mPositionInfoCall->resultAs<GetPositionInfoResponse>();
    updateCurrentTrack(response->trackUri(), response->trackMetaData());
}

void Renderer::updatePolling() noexcept
{
    auto const poll = mPositionTracked and mRenderer != nullptr and mState == State::Playing;
    if (poll and not mPolling) {
        mClock->wakeUpAt(mClock->now() + pollInterval);
    }
    mPolling = poll;
}

void Renderer::onClockWokeUp() noexcept
{
    if (not mPolling) {
        return;
    }

    mClock->wakeUpAt(mClock->now() + pollInterval);
    requestPositionInfo();
}

} // namespace Multimedia
