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
    : m_soapMessageTransmitter(soapMessageTransmitter)
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

    m_contentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
    m_contentDirectorySCPD = conDirectoryServiceValidator.scpd();
    mName = deviceDescription.friendlyName();
    if(!deviceDescription.icons().isEmpty())
    {
        mIconUrl = deviceDescription.icons().first().url();
    }
}

QString MediaServer::name() const noexcept
{
    return mName;
}

QUrl MediaServer::iconUrl() const noexcept
{
    return mIconUrl;
}

QSharedPointer<PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    auto action = m_contentDirectorySCPD.action("GetSortCapabilities");
    SoapMessageGenerator msgGen;

    auto xmlMessage = msgGen.generateXmlMessageBody(action, m_contentDirectoryServiceDescription.serviceType());

    auto soapCall = m_soapMessageTransmitter->sendSoapMessage(m_contentDirectoryServiceDescription.controlUrl(), action.name(),
                                                              m_contentDirectoryServiceDescription.serviceType(), xmlMessage);

    return QSharedPointer<PendingSoapCall>{ new PendingSoapCall{ soapCall } };
}

QSharedPointer<PendingSoapCall> MediaServer::browse(const QString &objectId, MediaServer::BrowseFlag browseFlag,
                                                    const QString &filter, const QString &sortCriteria) noexcept
{
    auto action = m_contentDirectorySCPD.action("Browse");

    ArgumentList browseArgs{ 6 };
    browseArgs << Argument{ "BrowseFlag", convertBrowseFlagToString(browseFlag) } << Argument{ "RequestedCount", "0" }
               << Argument{ "ObjectID", objectId } << Argument{ "Filter", filter } << Argument{ "StartingIndex", "0" }
               << Argument{ "SortCriteria", sortCriteria };

    SoapMessageGenerator msgGen;
    auto xmlMessage = msgGen.generateXmlMessageBody(action, m_contentDirectoryServiceDescription.serviceType(), browseArgs);

    auto soapCall = m_soapMessageTransmitter->sendSoapMessage(m_contentDirectoryServiceDescription.controlUrl(), action.name(),
                                                              m_contentDirectoryServiceDescription.serviceType(), xmlMessage);

    return QSharedPointer<PendingSoapCall>{ new PendingSoapCall{ soapCall } };
}

QString MediaServer::convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept
{
    return BrowseFlag::MetaData == browseFlag ? QStringLiteral("BrowseMetadata") : QStringLiteral("BrowseDirectChildren");
}

} // namespace UPnPAV
