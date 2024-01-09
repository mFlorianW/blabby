// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "MediaDevice.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{
/**
 * The MediaRenderer is facade for UPnPAV MediaRenderer device.
 */
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

/**
 * MediaRenderer simplifies the creation of the MediaRenderer and makes it possible
 * to replace real devices with e.g. test dummies.
 */
class BLABBYUPNPAV_EXPORT MediaRendererFactory
{
public:
    /**
     * Default constructor
     */
    MediaRendererFactory() = default;

    /**
     * Default virtual destructor
     */
    virtual ~MediaRendererFactory() = default;

    /**
     * Copy constructor
     */
    MediaRendererFactory(MediaRendererFactory const& other) = default;

    /**
     * Copy operator
     */
    MediaRendererFactory& operator=(MediaRendererFactory const& other) = default;

    /**
     * Move constructor
     */
    MediaRendererFactory(MediaRendererFactory&& other) = default;

    /**
     * Move operator
     */
    MediaRendererFactory& operator=(MediaRendererFactory&& other) = default;

    /**
     * Factory method that creates tries to create a @ref UPnPAV::MediaRenderer instance.
     * @throws InvalidDeviceDescription when the DeviceDescription doesn't contain the
     *         required functions and state variables.
     * @return The freestore allocated @ref UPnPAV::MediaRenderer
     */
    std::unique_ptr<MediaRenderer> create(DeviceDescription const& desc);
};

} // namespace UPnPAV
