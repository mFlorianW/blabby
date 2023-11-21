// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "MediaDevice.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{
class BLABBYUPNPAV_EXPORT MediaRenderer : public MediaDevice
{
public:
    /**
     * Creates an instance of the MediaRenderer.
     *
     * @param desc The DeviceDescription of the MediaRenderer.
     * @param msgTransmitter The transmitter for sending SOAP messages to the device.
     *
     * @throws InvalidDeviceDescription when the DeviceDescription doesn't contain the
     *         required functions and state variables.
     */
    MediaRenderer(DeviceDescription desc, QSharedPointer<SoapMessageTransmitter> msgTransmitter);
};

} // namespace UPnPAV
