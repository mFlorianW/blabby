// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaDevice.h"
#include "InvalidDeviceDescription.h"
#include "private/ConnectionManagerServiceValidator.h"
#include "private/MediaDevicePrivate.h"
#include "private/SoapMessageGenerator.h"

namespace UPnPAV
{

MediaDevice::MediaDevice(DeviceDescription const &deviceDescription,
                         QSharedPointer<SoapMessageTransmitter> msgTransmitter)
    : d{QScopedPointer<MediaDevicePrivate>(new MediaDevicePrivate{deviceDescription, msgTransmitter})}
{
    ConnectionManagerServiceValidator conManagerServiceValidator{deviceDescription};
    if (!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    if (!deviceDescription.icons().isEmpty())
    {
        d->mIconUrl = deviceDescription.icons().first().url();
    }

    d->mConnectionManagerDescription = conManagerServiceValidator.serviceDescription();
    d->mConnectionManagerSCPD = conManagerServiceValidator.scpd();
    d->mName = deviceDescription.friendlyName();
}

MediaDevice::~MediaDevice() = default;

const QString &MediaDevice::name() const noexcept
{
    return d->mName;
}

const QUrl &MediaDevice::iconUrl() const noexcept
{
    return d->mIconUrl;
}

QScopedPointer<PendingSoapCall> MediaDevice::protocolInfo() noexcept
{
    const auto action = d->mConnectionManagerSCPD.action("GetProtocolInfo");
    auto msgGen = SoapMessageGenerator{};
    const auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType());
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription.controlUrl(),
                                                                action.name(),
                                                                d->mConnectionManagerDescription.serviceType(),
                                                                xmlMessage);
    return QScopedPointer<PendingSoapCall>{new (std::nothrow) PendingSoapCall{soapCall}};
}

QScopedPointer<PendingSoapCall> MediaDevice::currentConnectionIds() noexcept
{
    const auto action = d->mConnectionManagerSCPD.action("GetCurrentConnectionIDs");
    auto msgGen = SoapMessageGenerator{};
    const auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType());
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription.controlUrl(),
                                                                action.name(),
                                                                d->mConnectionManagerDescription.serviceType(),
                                                                xmlMessage);
    return QScopedPointer<PendingSoapCall>{new (std::nothrow) PendingSoapCall{soapCall}};
}

QScopedPointer<PendingSoapCall> MediaDevice::currentConnectionInfo(quint32 connectionId) noexcept
{
    const auto action = d->mConnectionManagerSCPD.action("GetCurrentConnectionInfo");
    auto msgGen = SoapMessageGenerator{};

    const auto arg = Argument{.name = "ConnectionID", .value = QString::number(connectionId)};
    const auto xmlMessage =
        msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription.controlUrl(),
                                                                action.name(),
                                                                d->mConnectionManagerDescription.serviceType(),
                                                                xmlMessage);
    return QScopedPointer<PendingSoapCall>{new (std::nothrow) PendingSoapCall{soapCall}};
}

} // namespace UPnPAV
