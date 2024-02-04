// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DeviceDescription.hpp"
#include "blabbyupnpav_export.h"
#include <SoapBackend.hpp>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaRendererPrivate
{
public:
    MediaRendererPrivate(DeviceDescription deviceDescription, QSharedPointer<SoapBackend> transmitter)
        : mDeviceDescription(std::move(deviceDescription))
        , mSoapMessageTransmitter(std::move(transmitter))
    {
    }

    DeviceDescription mDeviceDescription;
    ServiceDescription mRenderingControlService;
    ServiceControlPointDefinition mRenderingControlSCPD;
    QSharedPointer<SoapBackend> mSoapMessageTransmitter;
};

} // namespace UPnPAV
