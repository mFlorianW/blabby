// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include "MediaDevice.h"
#include "PendingSoapCall.h"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>
#include <QUrl>

namespace UPnPAV
{
class SoapMessageTransmitter;
class DeviceDescription;
class MediaServerPrivate;

class BLABBYUPNPAV_EXPORT MediaServer : public MediaDevice
{
    Q_DISABLE_COPY_MOVE(MediaServer)
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
     * Creates a MediaService instance.
     *
     * @param deviceDescription The parsed XML description of
     *        the device which is used to check for the minimum required
     *        values and functions.
     * @param soapMessageTransmitter Transmitter to send out soap messages
     *        to a Media Server.
     *
     * @throws InvalidDeviceDescription if the given
     *         description doesn't contain the minimum
     *         requried functions and values.
     */
    MediaServer(const DeviceDescription &deviceDescription,
                const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter);
    ~MediaServer() override;

    /**
     * Request the supported sort capabilities of the media server.
     *
     * @return PendingSoapCall with the result or error.
     */
    virtual QSharedPointer<PendingSoapCall> getSortCapabilities() noexcept;

    /**
     * Browse the media server.
     *
     * @param objectId The object that shall be browsed.
     * @param browseFlag What shall be browsed, e.g. MetaData.
     * @param filter Comma seperated list of properties that shall appear in the result
     * @param sortCriteria Comma seperated list of in which order the result shall be returned
     * @return PendingSoapCall with the result or error.
     */
    virtual QSharedPointer<PendingSoapCall> browse(const QString &objectId,
                                                   BrowseFlag browseFlag,
                                                   const QString &filter,
                                                   const QString &sortCriteria) noexcept;

private:
    std::unique_ptr<MediaServerPrivate> d;
};

class BLABBYUPNPAV_EXPORT MediaServerFactory
{
    Q_DISABLE_COPY_MOVE(MediaServerFactory)
public:
    MediaServerFactory();
    ~MediaServerFactory();
    virtual std::unique_ptr<MediaServer> createMediaServer(const DeviceDescription &deviceDescription);
};

} // namespace UPnPAV

#endif // MEDIASERVER_H
