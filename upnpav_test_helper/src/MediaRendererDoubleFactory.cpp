// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRendererDoubleFactory.hpp"
#include "EventBackendDouble.hpp"
#include "SoapBackendDouble.hpp"

namespace UPnPAV::Doubles
{

std::unique_ptr<MediaRenderer> MediaRendererDoubleFactory::create(DeviceDescription const& desc)
{
    auto renderer = std::make_unique<MediaRendererDouble>(desc,
                                                          QSharedPointer<SoapBackendDouble>::create(),
                                                          QSharedPointer<EventBackend>::create());
    mRenderers.insert(desc.friendlyName(), renderer.get());
    return renderer;
}

MediaRendererDouble* MediaRendererDoubleFactory::renderer(QString const& friendlyName) const noexcept
{
    return mRenderers.value(friendlyName, nullptr);
}

} // namespace UPnPAV::Doubles
