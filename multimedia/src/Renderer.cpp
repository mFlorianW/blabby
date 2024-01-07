// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Renderer.hpp"
#include "GetProtocolInfoResponse.hpp"
#include "PendingSoapCall.hpp"
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
}

Renderer::~Renderer() = default;

QString const &Renderer::name() const noexcept
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

void Renderer::playback(Item const &item) noexcept
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

bool Renderer::isPlayableItem(Item const &item) const noexcept
{
    for (auto const &protocol : mProtocols) {
        for (auto const &itemProto : item.supportedTypes()) {
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
    if (mPlayCall and not mPlayCall->hasError()) {
        Q_EMIT playbackStarted();
    } else if (mPlayCall and mPlayCall->hasError()) {
        Q_EMIT playbackFailed(QString{"Failed to call play. Error code: %1. Error description: %2"}.arg(
            QVariant::fromValue<PendingSoapCall::ErrorCode>(mPlayCall->errorCode()).toString(),
            mPlayCall->errorDescription()));
    }
}

} // namespace Multimedia
