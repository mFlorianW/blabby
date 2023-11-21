// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVER_P_H
#define MEDIASERVER_P_H

#include "DeviceDescription.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"
#include "SoapMessageTransmitter.hpp"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>
#include <QUrl>
#include <utility>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaServerPrivate
{
public:
    MediaServerPrivate(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitter> transmitter)
        : mDeviceDescription(std::move(deviceDescription))
        , mSoapMessageTransmitter(std::move(transmitter))
    {
    }

    DeviceDescription mDeviceDescription;
    ServiceDescription mContentDirectoryServiceDescription;
    ServiceControlPointDefinition mContentDirectorySCPD;
    QSharedPointer<SoapMessageTransmitter> mSoapMessageTransmitter;
};

} // namespace UPnPAV

#endif // MEDIASERVER_P_H
