// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVER_P_H
#define MEDIASERVER_P_H

#include "DeviceDescription.h"
#include "ServiceControlPointDefinition.h"
#include "ServiceDescription.h"
#include "SoapMessageTransmitter.h"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>
#include <QUrl>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaServerPrivate
{
public:
    MediaServerPrivate(const DeviceDescription &deviceDescription, QSharedPointer<SoapMessageTransmitter> transmitter)
        : mDeviceDescription(deviceDescription)
        , mSoapMessageTransmitter(transmitter)
    {
    }

    static QString convertBrowseFlagToString(MediaServer::BrowseFlag browseFlag) noexcept;

    DeviceDescription mDeviceDescription;
    ServiceDescription mContentDirectoryServiceDescription;
    ServiceControlPointDefinition mContentDirectorySCPD;
    QSharedPointer<SoapMessageTransmitter> mSoapMessageTransmitter;
    QString mName{""};
    QUrl mIconUrl{""};
};

} // namespace UPnPAV

#endif // MEDIASERVER_P_H
