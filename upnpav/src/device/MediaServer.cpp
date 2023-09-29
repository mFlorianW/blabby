// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServer.h"
#include "DeviceDescription.h"
#include "HttpSoapMessageTransmitter.h"
#include "InvalidDeviceDescription.h"
#include "PendingSoapCall.h"
#include "SoapMessageTransmitter.h"
#include "private/ConnectionManagerServiceValidator.h"
#include "private/ContentDirectoryServiceValidator.h"
#include "private/MediaServer_p.h"
#include "private/SoapMessageGenerator.h"

namespace UPnPAV
{

MediaServerFactory::MediaServerFactory()
    : IMediaServerFactory()
{
}

MediaServerFactory::~MediaServerFactory() = default;

std::unique_ptr<IMediaServer> MediaServerFactory::createMediaServer(const DeviceDescription &deviceDescription)
{
    return std::make_unique<MediaServer>(deviceDescription,
                                         QSharedPointer<HttpSoapMessageTransmitter>{new HttpSoapMessageTransmitter()});
}

MediaServer::MediaServer(const DeviceDescription &deviceDescription,
                         const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter)
    : IMediaServer()
    , d(std::make_unique<MediaServerPrivate>(deviceDescription, soapMessageTransmitter))
{
    ConnectionManagerServiceValidator conManagerServiceValidator{deviceDescription};
    if (!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    ContentDirectoryServiceValidator conDirectoryServiceValidator{deviceDescription};
    if (!conDirectoryServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conDirectoryServiceValidator.errorMessage()};
    }

    d->mContentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
    d->mContentDirectorySCPD = conDirectoryServiceValidator.scpd();
    d->mConnectionManagerDescription = conManagerServiceValidator.serviceDescription();
    d->mConnectionManagerSCPD = conManagerServiceValidator.scpd();
    d->mName = deviceDescription.friendlyName();
    if (!deviceDescription.icons().isEmpty())
    {
        d->mIconUrl = deviceDescription.icons().first().url();
    }
}

MediaServer::~MediaServer() = default;

const QString &MediaServer::name() const noexcept
{
    return d->mName;
}

const QUrl &MediaServer::iconUrl() const noexcept
{
    return d->mIconUrl;
}

QScopedPointer<PendingSoapCall> MediaServer::protocolInfo() noexcept
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

QScopedPointer<PendingSoapCall> MediaServer::currentConnectionIds() noexcept
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

QScopedPointer<PendingSoapCall> MediaServer::currentConnectionInfo(quint32 connectionId) noexcept
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

QSharedPointer<PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    auto action = d->mContentDirectorySCPD.action("GetSortCapabilities");
    SoapMessageGenerator msgGen;

    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType());

    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription.controlUrl(),
                                                                action.name(),
                                                                d->mContentDirectoryServiceDescription.serviceType(),
                                                                xmlMessage);

    return QSharedPointer<PendingSoapCall>{new (std::nothrow) PendingSoapCall{soapCall}};
}

QSharedPointer<PendingSoapCall> MediaServer::browse(const QString &objectId,
                                                    MediaServer::BrowseFlag browseFlag,
                                                    const QString &filter,
                                                    const QString &sortCriteria) noexcept
{
    auto action = d->mContentDirectorySCPD.action("Browse");

    ArgumentList browseArgs{6};
    browseArgs << Argument{"BrowseFlag", d->convertBrowseFlagToString(browseFlag)} << Argument{"RequestedCount", "0"}
               << Argument{"ObjectID", objectId} << Argument{"Filter", filter} << Argument{"StartingIndex", "0"}
               << Argument{"SortCriteria", sortCriteria};

    SoapMessageGenerator msgGen;
    auto xmlMessage =
        msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType(), browseArgs);

    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription.controlUrl(),
                                                                action.name(),
                                                                d->mContentDirectoryServiceDescription.serviceType(),
                                                                xmlMessage);

    return QSharedPointer<PendingSoapCall>{new (std::nothrow) PendingSoapCall{soapCall}};
}

QString MediaServerPrivate::convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept
{
    return MediaServer::BrowseFlag::MetaData == browseFlag ? QStringLiteral("BrowseMetadata")
                                                           : QStringLiteral("BrowseDirectChildren");
}

} // namespace UPnPAV
