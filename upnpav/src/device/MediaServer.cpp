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
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "ConnectionManagerServiceValidator.h"
#include "ContentDirectoryServiceValidator.h"
#include "SoapMessageTransmitter.h"
#include "SoapMessageGenerator.h"

namespace UPnPAV
{

MediaServer::MediaServer(const DeviceDescription &deviceDescription,
                         const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter)
    : m_soapMessageTransmitter(soapMessageTransmitter)
{
    ConnectionManagerServiceValidator conManagerServiceValidator{deviceDescription};
    if(!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    ContentDirectoryServiceValidator conDirectoryServiceValidator{deviceDescription};
    if(!conDirectoryServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conDirectoryServiceValidator.errorMessage()};
    }

    m_contentDirectoryServiceDescription = conDirectoryServiceValidator.serviceDescription();
}

QSharedPointer<PendingSoapCall> MediaServer::getSortCapabilities()
{
    static const QString actionName{"GetSortCapabilities"};
    SoapMessageGenerator msgGen;

    auto xmlMessage = msgGen.generateXmlMessageBody(actionName,
                                                    m_contentDirectoryServiceDescription.serviceType());

    auto soapCall = m_soapMessageTransmitter->sendSoapMessage(m_contentDirectoryServiceDescription.controlUrl(),
                                                              actionName,
                                                              m_contentDirectoryServiceDescription.serviceType(),
                                                              xmlMessage);

    return QSharedPointer<PendingSoapCall>
    {
        new PendingSoapCall
        {
            soapCall
        }
    };
}

QSharedPointer<PendingSoapCall> MediaServer::browse(const QString &objectId,
                                 MediaServer::BrowseFlag browseFlag,
                                 const QString &filter,
                                 const QString &sortCriteria)
{
    static const QString actionName{"Browse"};
    SoapMessageGenerator msgGen;

    ArgumentMap browseArgs
    {
        std::make_pair("BrowseFlag", convertBrowseFlagToString(browseFlag)),
        std::make_pair("ObjectID", objectId),
        std::make_pair("Filter", filter),
        std::make_pair("RequestedCount", "0"),
        std::make_pair("SortCriteria", sortCriteria),
        std::make_pair("StartingIndex", "0")
    };

    auto xmlMessage = msgGen.generateXmlMessageBody(actionName,
                                                    m_contentDirectoryServiceDescription.serviceType(),
                                                    browseArgs);

    auto soapCall = m_soapMessageTransmitter->sendSoapMessage(m_contentDirectoryServiceDescription.controlUrl(),
                                                              actionName,
                                                              m_contentDirectoryServiceDescription.serviceType(),
                                                              xmlMessage);

    return QSharedPointer<PendingSoapCall>
    {
        new PendingSoapCall
        {
            soapCall
        }
    };
}

QString MediaServer::convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept
{
    return BrowseFlag::MetaData == browseFlag ? QStringLiteral("BrowseMetadata")
                                              : QStringLiteral("BrowseDirectChildren");
}

} //namespace UPnPAV
