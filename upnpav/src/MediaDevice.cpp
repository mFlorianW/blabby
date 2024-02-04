// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaDevice.hpp"
#include "InvalidDeviceDescription.hpp"
#include "private/AvTransportServiceValidator.hpp"
#include "private/ConnectionManagerServiceValidator.hpp"
#include "private/EventPropertyReader.hpp"
#include "private/LastChangeReader.hpp"
#include "private/LoggingCategories.hpp"
#include "private/MediaDevicePrivate.hpp"
#include "private/SoapMessageGenerator.hpp"

namespace UPnPAV
{

namespace
{

MediaDevice::State deviceState(QString const& rawState)
{
    if (rawState == QStringLiteral("STOPPED")) {
        return MediaDevice::State::Stopped;
    } else if (rawState == QStringLiteral("PAUSED_PLAYBACK")) {
        return MediaDevice::State::PausedPlayback;
    } else if (rawState == QStringLiteral("PAUSED_RECORDING")) {
        return MediaDevice::State::PausedRecording;
    } else if (rawState == QStringLiteral("PLAYING")) {
        return MediaDevice::State::Playing;
    } else if (rawState == QStringLiteral("RECORDING")) {
        return MediaDevice::State::Recording;
    } else if (rawState == QStringLiteral("TRANSITIONING")) {
        return MediaDevice::State::Transitioning;
    } else if (rawState == QStringLiteral("NO_MEDIA_PRESENT")) {
        return MediaDevice::State::NoMediaPresent;
    }

    qCWarning(upnpavEvent) << "Unknown Transport state" << rawState << "received.";
    return MediaDevice::State::NoMediaPresent;
}

} // namespace

MediaDevice::MediaDevice(DeviceDescription deviceDescription,
                         QSharedPointer<SoapBackend> soapBackend,
                         QSharedPointer<EventBackend> eventBackend)
    : d{QScopedPointer<MediaDevicePrivate>(
          new MediaDevicePrivate{std::move(deviceDescription), std::move(soapBackend), std::move(eventBackend), *this})}
{
    ConnectionManagerServiceValidator conManagerServiceValidator{d->mDeviceDescription};
    if (!conManagerServiceValidator.validate()) {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    if (!d->mDeviceDescription.icons().isEmpty()) {
        d->mIconUrl = d->mDeviceDescription.icons().first().url();
    }

    d->mConnectionManagerDescription = conManagerServiceValidator.serviceDescription();
    d->mConnectionManagerSCPD = conManagerServiceValidator.scpd();
    d->mName = d->mDeviceDescription.friendlyName();

    auto const hasAvTransportService = d->mDeviceDescription.service("AVTransport").has_value();
    if (hasAvTransportService) {
        auto avSerVali = AvTransportServiceValidator{d->mDeviceDescription};
        if (!avSerVali.validate()) {
            throw InvalidDeviceDescription{avSerVali.errorMessage()};
        }
        d->mHasAvTransportService = true;
        d->mAvTransportDescription = avSerVali.serviceDescription();
        d->mAvTransportDescriptionSCPD = avSerVali.scpd();

        d->mAvTransportEvents = d->mEventBackend->subscribeEvents(d->mAvTransportDescription);
        QObject::connect(d->mAvTransportEvents.get(),
                         &EventSubscriptionHandle::propertiesChanged,
                         d->mAvTransportEvents.get(),
                         [this]() {
                             auto reader = EventPropertyReader{d->mAvTransportEvents->responseBody()};
                             if (not reader.read()) {
                                 return;
                             }

                             auto lastChangeValue = reader.property(QStringLiteral("LastChange"));
                             if (not lastChangeValue.has_value()) {
                                 return;
                             }

                             auto lastChangeReader = LastChangeReader{lastChangeValue.value()};
                             if (not lastChangeReader.read()) {
                                 return;
                             }

                             auto const& instanceVariables = lastChangeReader.instanceVariables();
                             if (instanceVariables.contains(QStringLiteral("0"))) {
                                 auto varialbes = instanceVariables.value(QStringLiteral("0"));
                                 if (varialbes.contains(QStringLiteral("TransportState"))) {
                                     d->setState(deviceState(varialbes.value(QStringLiteral("TransportState"))));
                                 }
                             }
                         });
    }
}

MediaDevice::~MediaDevice() = default;

QString const& MediaDevice::name() const noexcept
{
    return d->mName;
}

QUrl const& MediaDevice::iconUrl() const noexcept
{
    return d->mIconUrl;
}

MediaDevice::State MediaDevice::state() const noexcept
{
    return d->mState;
}

std::unique_ptr<PendingSoapCall> MediaDevice::protocolInfo() noexcept
{
    auto const action = d->mConnectionManagerSCPD.action("GetProtocolInfo");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType());
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription,
                                                                d->mConnectionManagerSCPD,
                                                                action,
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::unique_ptr<PendingSoapCall> MediaDevice::currentConnectionIds() noexcept
{
    auto const action = d->mConnectionManagerSCPD.action("GetCurrentConnectionIDs");
    auto msgGen = SoapMessageGenerator{};
    auto const xmlMessage = msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType());
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription.controlUrl(),
                                                                action.name(),
                                                                d->mConnectionManagerDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::unique_ptr<PendingSoapCall> MediaDevice::currentConnectionInfo(quint32 connectionId) noexcept
{
    auto const action = d->mConnectionManagerSCPD.action("GetCurrentConnectionInfo");
    auto msgGen = SoapMessageGenerator{};

    auto const arg = Argument{.name = "ConnectionID", .value = QString::number(connectionId)};
    auto const xmlMessage =
        msgGen.generateXmlMessageBody(action, d->mConnectionManagerDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mConnectionManagerDescription.controlUrl(),
                                                                action.name(),
                                                                d->mConnectionManagerDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

bool MediaDevice::hasAvTransportService() const noexcept
{
    return d->mHasAvTransportService;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::setAvTransportUri(quint32 instanceId,
                                                                               QString const& uri,
                                                                               QString const& uriMetaData) noexcept
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const args = QVector<Argument>{
        {.name = "InstanceID", .value = QString::number(instanceId)},
        {.name = "CurrentURI", .value = uri},
        {.name = "CurrentURIMetaData", .value = uriMetaData},
    };
    auto const action = d->mAvTransportDescriptionSCPD.action("SetAVTransportURI");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), args);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::mediaInfo(quint32 instanceId)
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("GetMediaInfo");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("GetTransportInfo");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("GetPositionInfo");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("GetDeviceCapabilities");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("GetTransportSettings");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("Stop");
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
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const args = QVector<Argument>{Argument{.name = "InstanceID", .value = QString::number(instanceId)},
                                        Argument{.name = "Speed", .value = QStringLiteral("1")}};
    auto const action = d->mAvTransportDescriptionSCPD.action("Play");
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
                                                                  QString const& target)
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const args = QVector<Argument>{Argument{.name = "InstanceID", .value = QString::number(instanceId)},
                                        Argument{.name = "Unit", .value = QString::number(static_cast<qint32>(mode))},
                                        Argument{.name = "Target", .value = target}};
    auto const action = d->mAvTransportDescriptionSCPD.action("Seek");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), args);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::next(quint32 instanceId)
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("Next");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::previous(quint32 instanceId)
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("Previous");
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaDevice::pause(quint32 instanceId) noexcept
{
    if (not hasAvTransportService()) {
        return std::nullopt;
    }

    auto const arg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const action = d->mAvTransportDescriptionSCPD.action("Pause");
    if (action.name().isEmpty()) {
        return std::nullopt;
    }
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mAvTransportDescription.serviceType(), {arg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mAvTransportDescription.controlUrl(),
                                                                action.name(),
                                                                d->mAvTransportDescription.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

} // namespace UPnPAV
