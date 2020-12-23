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
#ifndef IMEDIASERVER_H
#define IMEDIASERVER_H

#include "UPnP_Export.h"
#include <QSharedPointer>
#include <memory>

namespace UPnPAV
{
class PendingSoapCall;
class DeviceDescription;
class SoapMessageTransmitter;

/**
 * Interface definition for an UPnP MediaServer.
 */
class UPNP_EXPORT IMediaServer : public QObject
{
    Q_OBJECT
public:
    /**
     * Flags for browse request.
     */
    enum BrowseFlag
    {
        MetaData,
        DirectChildren
    };

    /**
     * Default destructor
     */
    virtual ~IMediaServer() = default;

    /**
     * Gives the name of the media server.
     *
     * @return The name of the media server.
     */
    virtual QString name() const noexcept = 0;

    /**
     * Gives a icon url of the media server.
     *
     * @return The icon url of the media server.
     */
    virtual QUrl iconUrl() const noexcept = 0;

    /**
     * Request the supported sort capabilities of the media server.
     *
     * @return PendingSoapCall with the result or error.
     */
    virtual QSharedPointer<PendingSoapCall> getSortCapabilities() noexcept = 0;

    /**
     * Browse the media server.
     *
     * @param objectId The object that shall be browsed.
     * @param browseFlag What shall be browsed, e.g. MetaData.
     * @param filter Comma seperated list of properties that shall appear in the result
     * @param sortCriteria Comma seperated list of in which order the result shall be returned
     * @return PendingSoapCall with the result or error.
     */
    virtual QSharedPointer<PendingSoapCall> browse(const QString &objectId, BrowseFlag browseFlag,
                                                   const QString &filter, const QString &sortCriteria) noexcept = 0;
};

class UPNP_EXPORT IMediaServerFactory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(IMediaServerFactory)
public:
    virtual ~IMediaServerFactory() = default;

    /**
     * Creates a MediaServer instance.
     *
     * @param deviceDescription The device description of which the media server should use.
     * @return std::unique_ptr<IMediaServer>
     *
     * @Throws InvalidDeviceDescription when the MediaServer can't parse the description.
     */
    virtual std::unique_ptr<IMediaServer> createMediaServer(const DeviceDescription &deviceDescription) = 0;

protected:
    IMediaServerFactory() = default;
};

} // namespace UPnPAV

#endif // IMEDIASERVER_H
