// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaRendererDouble.hpp"
#include "ProtocolInfoResponse.hpp"

namespace UPnPAV::Doubles
{

MediaRendererDouble::MediaRendererDouble(DeviceDescription desc, QSharedPointer<SoapMessageTransmitter> transmitter)
    : UPnPAV::MediaRenderer{std::move(desc), std::move(transmitter)}
{
    mProtoInfoCall->setRawMessage(ValidProtoclInfoResponseOfRenderer);
}

MediaRendererDouble::~MediaRendererDouble() = default;

std::unique_ptr<PendingSoapCall> MediaRendererDouble::protocolInfo() noexcept
{
    mIsProtocolInfoCalled = true;
    return std::make_unique<PendingSoapCall>(mProtoInfoCall);
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::protocolInfoCall() const noexcept
{
    return mProtoInfoCall;
}

bool MediaRendererDouble::isProtocolInfoCalled() const noexcept
{
    return mIsProtocolInfoCalled;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::setAvTransportUri(
    quint32 instanceId,
    QString const &uri,
    QString const &uriMetaData) noexcept
{
    mIsSetAvTranstportUriCalled = true;
    mSetAvTransportUriData = AvTransportUriData{.instanceId = instanceId, .uri = uri, .uriMetaData = uriMetaData};
    return std::make_unique<PendingSoapCall>(mSetAvTransportUriCall);
}

bool MediaRendererDouble::isSetAvTransportUriCalled() const noexcept
{
    return mIsSetAvTranstportUriCalled;
}

AvTransportUriData MediaRendererDouble::avTransportUriData() const noexcept
{
    return mSetAvTransportUriData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::avTransportUriCall() const noexcept
{
    return mSetAvTransportUriCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::play(quint32 instanceId)
{
    mIsPlayCalled = true;
    mPlayData = PlayData{.instanceId = instanceId};
    return std::make_unique<PendingSoapCall>(mPlayCall);
}

bool MediaRendererDouble::isPlayCalled() const noexcept
{
    return mIsPlayCalled;
}

PlayData MediaRendererDouble::playData() const noexcept
{
    return mPlayData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::playCall() const noexcept
{
    return mPlayCall;
}

} // namespace UPnPAV::Doubles
