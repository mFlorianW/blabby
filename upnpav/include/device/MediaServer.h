// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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

class UPNP_EXPORT MediaServerFactory : public IMediaServerFactory
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerFactory)
public:
    MediaServerFactory();
    ~MediaServerFactory() override;
    std::unique_ptr<IMediaServer> createMediaServer(const DeviceDescription &deviceDescription) override;
};

class UPNP_EXPORT MediaServer final : public IMediaServer
{
    Q_DISABLE_COPY_MOVE(MediaServer)
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
     *         description doesn't contain the minimum
     *         requried functions and values.
     */
    MediaServer(const DeviceDescription &deviceDescription, const QSharedPointer<SoapMessageTransmitter> &soapMessageTransmitter);

    const QString &name() const noexcept override;

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
