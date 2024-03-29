// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServer.hpp"
#include "DeviceDescription.hpp"
#include "HttpEventBackend.hpp"
#include "HttpSoapBackend.hpp"
#include "InvalidDeviceDescription.hpp"
#include "PendingSoapCall.hpp"
#include "private/ContentDirectoryServiceValidator.hpp"
#include "private/MediaServer_p.hpp"
#include "private/SoapMessageGenerator.hpp"

namespace UPnPAV
{

namespace
{
QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept;
}

MediaServerFactory::MediaServerFactory() = default;
MediaServerFactory::~MediaServerFactory() = default;

std::unique_ptr<MediaServer> MediaServerFactory::createMediaServer(DeviceDescription const& deviceDescription)
{
    return std::make_unique<MediaServer>(deviceDescription,
                                         QSharedPointer<HttpSoapBackend>::create(),
                                         QSharedPointer<HttpEventBackend>::create());
}

MediaServer::MediaServer(DeviceDescription const& deviceDescription,
                         QSharedPointer<SoapBackend> const& soapBackend,
                         QSharedPointer<EventBackend> eventBackend)
    : MediaDevice{deviceDescription, soapBackend, std::move(eventBackend)}
    , d(std::make_unique<MediaServerPrivate>(deviceDescription, soapBackend))
{
    ContentDirectoryServiceValidator conDirectoryServiceValidator{deviceDescription};
    if (!conDirectoryServiceValidator.validate()) {
        throw InvalidDeviceDescription{conDirectoryServiceValidator.errorMessage()};
    }

    d->mContentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
    d->mContentDirectorySCPD = conDirectoryServiceValidator.scpd();
}

MediaServer::~MediaServer() = default;

std::unique_ptr<PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    auto action = d->mContentDirectorySCPD.action("GetSortCapabilities");
    SoapMessageGenerator msgGen;

    auto xmlMessage = msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType());

    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription.controlUrl(),
                                                                action.name(),
                                                                d->mContentDirectoryServiceDescription.serviceType(),
                                                                xmlMessage);

    return std::make_unique<PendingSoapCall>(soapCall);
}

std::unique_ptr<PendingSoapCall> MediaServer::browse(QString const& objectId,
                                                     MediaServer::BrowseFlag browseFlag,
                                                     QString const& filter,
                                                     QString const& sortCriteria) noexcept
{
    auto action = d->mContentDirectorySCPD.action("Browse");

    ArgumentList browseArgs{6};
    browseArgs << Argument{"BrowseFlag", convertBrowseFlagToString(browseFlag)} << Argument{"RequestedCount", "0"}
               << Argument{"ObjectID", objectId} << Argument{"Filter", filter} << Argument{"StartingIndex", "0"}
               << Argument{"SortCriteria", sortCriteria};

    SoapMessageGenerator msgGen;
    auto xmlMessage =
        msgGen.generateXmlMessageBody(action, d->mContentDirectoryServiceDescription.serviceType(), browseArgs);
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mContentDirectoryServiceDescription,
                                                                d->mContentDirectorySCPD,
                                                                d->mContentDirectorySCPD.action("Browse"),
                                                                xmlMessage);

    return std::make_unique<PendingSoapCall>(soapCall);
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
