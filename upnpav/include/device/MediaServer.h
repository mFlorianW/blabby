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

#include "IMediaServer.h"
#include "PendingSoapCall.h"
#include "ServiceControlPointDefinition.h"
#include "ServiceDescription.h"
#include "UPnP_Export.h"
#include <QSharedPointer>
#include <QUrl>

namespace UPnPAV
{
class SoapMessageTransmitter;
class DeviceDescription;

class UPNP_EXPORT MediaServer final : public IMediaServer
{
public:
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
    MediaServer(const DeviceDescription &deviceDescription, const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter);

    /**
     * Gives the name of the media server.
     *
     * @return The name of the media server.
     */
    QString name() const noexcept override;

    /**
     * Gives a icon url of the media server.
     *
     * @return The icon url of the media server.
     */
    QUrl iconUrl() const noexcept override;

    /**
     * Request the supported sort capabilities of the media server.
     *
     * @return PendingSoapCall with the result or error.
     */
    QSharedPointer<PendingSoapCall> getSortCapabilities() noexcept override;

    /**
     * Browse the media server.
     *
     * @param objectId The object that shall be browsed.
     * @param browseFlag What shall be browsed, e.g. MetaData.
     * @param filter Comma seperated list of properties that shall appear in the result
     * @param sortCriteria Comma seperated list of in which order the result shall be returned
     * @return PendingSoapCall with the result or error.
     */
    QSharedPointer<PendingSoapCall> browse(const QString &objectId, BrowseFlag browseFlag, const QString &filter,
                                           const QString &sortCriteria) noexcept override;

private:
    static QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept;

private:
    ServiceDescription m_contentDirectoryServiceDescription;
    ServiceControlPointDefinition m_contentDirectorySCPD;
    QSharedPointer<SoapMessageTransmitter> m_soapMessageTransmitter;
    QString mName;
    QUrl mIconUrl;
};

} // namespace UPnPAV

#endif // MEDIASERVER_H
