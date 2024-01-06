// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRenderer.hpp"
#include "DeviceDescription.hpp"
#include "HttpSoapBackend.hpp"
#include "InvalidDeviceDescription.hpp"
#include "private/RenderingControlServiceValidator.hpp"

namespace UPnPAV
{

MediaRenderer::MediaRenderer(DeviceDescription desc, QSharedPointer<SoapBackend> msgTransmitter)
    : MediaDevice{desc, msgTransmitter}
{
    auto validator = RenderingControlServiceValidator{desc};
    if (!validator.validate()) {
        throw InvalidDeviceDescription(validator.errorMessage());
    }
}

std::unique_ptr<MediaRenderer> MediaRendererFactory::create(DeviceDescription const& desc)
{
    return std::make_unique<MediaRenderer>(desc, QSharedPointer<HttpSoapBackend>::create());
}

} // namespace UPnPAV
