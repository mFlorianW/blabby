// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Renderer.hpp"
#include "GetProtocolInfoResponse.hpp"
#include "GetVolumeResponse.hpp"
#include "PendingSoapCall.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <QVariant>

using namespace UPnPAV;

namespace Multimedia
{

namespace
{
constexpr auto defaultInstanceId = quint32{0};

RememberedRenderer rememberedOf(UPnPAV::MediaRenderer const& device)
{
    return RememberedRenderer{.identity = device.udn(),
                              .name = device.name(),
                              .manufacturer = device.manufacturer(),
                              .modelName = device.modelName(),
                              .address = device.address()};
}
} // namespace

Renderer::Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer)
    : mRenderer{std::move(mediaRenderer)}
{
    Q_ASSERT(mRenderer != nullptr);
    mRemembered = rememberedOf(*mRenderer);
    connectDevice();
}

Renderer::Renderer(RememberedRenderer remembered)
    : mRemembered{std::move(remembered)}
{
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
    setState(mRenderer->state());
    if (mInitialized) {
        initialize();
    }

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
    Q_EMIT availabilityChanged();
    updateVolume(0);

    if (mState != State::NoMedia) {
        mState = State::NoMedia;
        Q_EMIT stateChanged();
    }
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
}

void Renderer::initialize() noexcept
{
    if (mRenderer == nullptr) {
        qCWarning(mmRenderer) << "Failed to initialize Renderer" << mRemembered.name << "Error: it is Offline";
        return;
    }

    mInitialized = true;

    mProtoInfoCall = mRenderer->protocolInfo();
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
        mVolumeCall = std::move(call.value());
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
        mSetAvTransportUriCall = std::move(uriCall.value());
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
            mPlayCall = std::move(playCall.value());
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
        mStopCall = std::move(stopCall.value());
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
            mResumeCall = std::move(resumeCall.value());
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
        mSetVolumeCall = std::move(call.value());
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

} // namespace Multimedia
