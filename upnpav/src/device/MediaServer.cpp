// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServer.h"
#include "ConnectionManagerServiceValidator.h"
#include "ContentDirectoryServiceValidator.h"
#include "DeviceDescription.h"
#include "HttpSoapMessageTransmitter.h"
#include "InvalidDeviceDescription.h"
#include "MediaServer_p.h"
#include "SoapMessageGenerator.h"
#include "SoapMessageTransmitter.h"

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
                                         QSharedPointer<HttpSoapMessageTransmitter>{ new HttpSoapMessageTransmitter() });
}

MediaServer::MediaServer(const DeviceDescription &deviceDescription, const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter)
    : IMediaServer()
    , d(std::make_unique<MediaServerPrivate>(deviceDescription, soapMessageTransmitter))
{
    ConnectionManagerServiceValidator conManagerServiceValidator{ deviceDescription };
    if(!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{ conManagerServiceValidator.errorMessage() };
    }

    ContentDirectoryServiceValidator conDirectoryServiceValidator{ deviceDescription };
    if(!conDirectoryServiceValidator.validate())
    {
        throw InvalidDeviceDescription{ conDirectoryServiceValidator.errorMessage() };
    }

    d->mContentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
    d->mContentDirectorySCPD = conDirectoryServiceValidator.scpd();
    d->mName = deviceDescription.friendlyName();
    if(!deviceDescription.icons().isEmpty())
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

QSharedPointer<PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    auto action = d->mContentDirectorySCPD.action("GetSortCapabilities");
    SoapMessageGenerator msgGen;

    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType());

    auto soapCall =
        d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription.controlUrl(), action.name(),
                                                    d->mContentDirectoryServiceDescription.serviceType(), xmlMessage);

    return QSharedPointer<PendingSoapCall>{ new PendingSoapCall{ soapCall } };
}

QSharedPointer<PendingSoapCall> MediaServer::browse(const QString &objectId, MediaServer::BrowseFlag browseFlag,
                                                    const QString &filter, const QString &sortCriteria) noexcept
{
    auto action = d->mContentDirectorySCPD.action("Browse");

    ArgumentList browseArgs{ 6 };
    browseArgs << Argument{ "BrowseFlag", d->convertBrowseFlagToString(browseFlag) }
               << Argument{ "RequestedCount", "0" } << Argument{ "ObjectID", objectId } << Argument{ "Filter", filter }
               << Argument{ "StartingIndex", "0" } << Argument{ "SortCriteria", sortCriteria };

    SoapMessageGenerator msgGen;
    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType(), browseArgs);

    auto soapCall =
        d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription.controlUrl(), action.name(),
                                                    d->mContentDirectoryServiceDescription.serviceType(), xmlMessage);

    return QSharedPointer<PendingSoapCall>{ new PendingSoapCall{ soapCall } };
}

QString MediaServerPrivate::convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept
{
    return MediaServer::BrowseFlag::MetaData == browseFlag ? QStringLiteral("BrowseMetadata") :
                                                             QStringLiteral("BrowseDirectChildren");
}

} // namespace UPnPAV
