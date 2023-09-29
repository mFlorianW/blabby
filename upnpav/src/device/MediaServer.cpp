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

namespace
{
QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept;
}

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
    : IMediaServer{deviceDescription, soapMessageTransmitter}
    , d(std::make_unique<MediaServerPrivate>(deviceDescription, soapMessageTransmitter))
{
    ContentDirectoryServiceValidator conDirectoryServiceValidator{deviceDescription};
    if (!conDirectoryServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conDirectoryServiceValidator.errorMessage()};
    }

    d->mContentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
    d->mContentDirectorySCPD = conDirectoryServiceValidator.scpd();
}

MediaServer::~MediaServer() = default;

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
    browseArgs << Argument{"BrowseFlag", convertBrowseFlagToString(browseFlag)} << Argument{"RequestedCount", "0"}
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

namespace
{

QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept
{
    return MediaServer::BrowseFlag::MetaData == browseFlag ? QStringLiteral("BrowseMetadata")
                                                           : QStringLiteral("BrowseDirectChildren");
}

} // namespace

} // namespace UPnPAV
