// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaRenderer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "private/RenderingControlServiceValidator.h"

namespace UPnPAV
{

MediaRenderer::MediaRenderer(DeviceDescription desc, QSharedPointer<SoapMessageTransmitter> msgTransmitter)
    : MediaDevice{desc, msgTransmitter}
{
    auto validator = RenderingControlServiceValidator{desc};
    if (!validator.validate())
    {
        throw InvalidDeviceDescription(validator.errorMessage());
    }
}

} // namespace UPnPAV
