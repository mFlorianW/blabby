// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Renderer.hpp"
#include "GetProtocolInfoResponse.hpp"
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
}

Renderer::Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer)
    : mRenderer{std::move(mediaRenderer)}
{
    Q_ASSERT(mRenderer != nullptr);

    connect(mRenderer.get(), &MediaRenderer::stateChanged, this, [this]() {
        setState(mRenderer->state());
    });
}

Renderer::~Renderer() = default;

QString const& Renderer::name() const noexcept
{
    return mRenderer->name();
}

QString Renderer::iconUrl() const noexcept
{
    return mRenderer->iconUrl().toString();
}

void Renderer::initialize() noexcept
{
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
}

void Renderer::playback(Item const& item) noexcept
{
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
    if (mState == State::Stopped or mState == State::Paused) {
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

} // namespace Multimedia
