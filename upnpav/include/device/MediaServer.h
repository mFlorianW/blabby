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
#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include "UPnP_Export.h"
#include "ServiceDescription.h"
#include "PendingReply.h"

#include <QSharedPointer>

namespace UPnPAV
{
class DeviceDescription;
class SoapMessageTransmitter;

class UPNP_EXPORT MediaServer final
{
public:
    enum BrowseFlag
    {
        MetaData,
        DirectChildren
    };

    /**
     * Creates a MediaService instance.
     *
     * @param deviceDescription The parsed XML description of
     *        the device which is used to check for the minimum required
     *        values and functions.
     * @param soapMessageTransmitter Transmitter to send out soap messages
     *        to a Media Server.
     *
     * @throws InvalidDeviceDescription if the given
     *         description contain the minimum requried
     *         functions and values.
     */
    MediaServer(const DeviceDescription &deviceDescription,
                const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter);

    PendingReply getSortCapabilities();

    PendingReply browse(const QString &objectId,
                        BrowseFlag browseFlag,
                        const QString &filter,
                        const QString &sortCriteria);

private:
    static QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept;

private:
    ServiceDescription m_contentDirectoryServiceDescription;
    QSharedPointer<SoapMessageTransmitter> m_soapMessageTransmitter;
};

} //namespace UPnPAV

#endif // MEDIASERVER_H
