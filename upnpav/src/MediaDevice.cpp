// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaDevice.h"
#include "InvalidDeviceDescription.h"
#include "private/AvTransportServiceValidator.h"
#include "private/ConnectionManagerServiceValidator.h"
#include "private/MediaDevicePrivate.h"
#include "private/SoapMessageGenerator.h"

namespace UPnPAV
{

MediaDevice::MediaDevice(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitter> msgTransmitter)
    : d{QScopedPointer<MediaDevicePrivate>(
          new MediaDevicePrivate{std::move(deviceDescription), std::move(msgTransmitter)})}
{
    ConnectionManagerServiceValidator conManagerServiceValidator{d->mDeviceDescription};
    if (!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    if (!d->mDeviceDescription.icons().isEmpty())
    {
        d->mIconUrl = d->mDeviceDescription.icons().first().url();
    }

    d->mConnectionManagerDescription = conManagerServiceValidator.serviceDescription();
    d->mConnectionManagerSCPD = conManagerServiceValidator.scpd();
    d->mName = d->mDeviceDescription.friendlyName();

    const auto hasAvTransportService = d->mDeviceDescription.service("AVTransport").has_value();
    if (hasAvTransportService)
    {
        auto avSerVali = AvTransportServiceValidator{d->mDeviceDescription};
        if (!avSerVali.validate())
        {
            throw InvalidDeviceDescription{avSerVali.errorMessage()};
        }
        d->mHasAvTransportService = true;
        d->mAvTransportDescription = avSerVali.serviceDescription();
        d->mAvTransportDescriptionSCPD = avSerVali.scpd();
    }
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

bool MediaDevice::hasAvTransportService() const noexcept
{
    return d->mHasAvTransportService;
}

std::optional<QScopedPointer<PendingSoapCall>> MediaDevice::setAvTransportUri(quint32 instanceId,
                                                                              QString const &uri,
                                                                              QString const &uriMetaData) noexcept
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto args = QVector<Argument>{
        {.name = "InstanceID", .value = QString::number(instanceId)},
        {.name = "CurrentURI", .value = uri},
        {.name = "CurrentURIMetaData", .value = uriMetaData},
    };
    const auto action = d->mAvTransportDescriptionSCPD.action("SetAVTransportURI");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), args);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::optional<QScopedPointer<PendingSoapCall>>(new (std::nothrow) PendingSoapCall{soapCall});
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::mediaInfo(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("GetMediaInfo");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::transportInfo(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("GetTransportInfo");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::positionInfo(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("GetPositionInfo");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::deviceCapilities(quint32 instanceId)
{

    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("GetDeviceCapabilities");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::transportSettings(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("GetTransportSettings");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::stop(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    const auto action = d->mAvTransportDescriptionSCPD.action("Stop");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::play(quint32 instanceId)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto args = QVector<Argument>{Argument{.name = "InstanceID", .value = QString::number(instanceId)},
                                        Argument{.name = "Speed", .value = QStringLiteral("1")}};
    const auto action = d->mAvTransportDescriptionSCPD.action("Play");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), args);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::seek(quint32 instanceId,
                                                                  SeekMode mode,
                                                                  QString const &target)
{
    if (not hasAvTransportService())
    {
        return std::nullopt;
    }

    const auto args = QVector<Argument>{Argument{.name = "InstanceID", .value = QString::number(instanceId)},
                                        Argument{.name = "Unit", .value = QString::number(static_cast<qint32>(mode))},
                                        Argument{.name = "Target", .value = target}};
    const auto action = d->mAvTransportDescriptionSCPD.action("Seek");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), args);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}
} // namespace UPnPAV
