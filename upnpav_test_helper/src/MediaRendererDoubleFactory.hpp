// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "MediaRendererDouble.hpp"
#include <QHash>

namespace UPnPAV::Doubles
{

/**
 * Creates @ref UPnPAV::Doubles::MediaRendererDouble instances instead of real MediaRenderers,
 * so tests can control the device behind a Renderer, e.g. its state.
 */
class MediaRendererDoubleFactory : public UPnPAV::MediaRendererFactory
{
public:
    /**
     * @copydoc UPnPAV::MediaRendererFactory::create
     */
    std::unique_ptr<MediaRenderer> create(DeviceDescription const& desc) override;

    /**
     * Gives the last created @ref UPnPAV::Doubles::MediaRendererDouble with the passed friendly name.
     * @note The factory doesn't own the double, the pointer dangles once the created renderer is destroyed.
     * @return The double or nullptr when no double with that name was created.
     */
    MediaRendererDouble* renderer(QString const& friendlyName) const noexcept;

private:
    QHash<QString, MediaRendererDouble*> mRenderers;
};

} // namespace UPnPAV::Doubles
