// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef IMEDIASERVER_H
#define IMEDIASERVER_H

#include "IMediaDevice.h"
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
class UPNP_EXPORT IMediaServer : public IMediaDevice
{
    Q_GADGET
    Q_DISABLE_COPY_MOVE(IMediaServer)
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

protected:
    IMediaServer() = default;
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
