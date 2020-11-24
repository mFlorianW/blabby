/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "MediaServer.h"
#include "ConnectionManagerServiceValidator.h"
#include "ContentDirectoryServiceValidator.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "SoapMessageGenerator.h"
#include "SoapMessageTransmitter.h"

namespace UPnPAV
{

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
}

QString MediaServer::name() const noexcept
{
    return mName;
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
