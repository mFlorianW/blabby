// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "IServiceProvider.hpp"
#include "MediaRenderer.hpp"
#include "Renderer.hpp"
#include "ServiceProvider.hpp"
#include "blabbymultimedia_export.h"
#include <QHash>
#include <QObject>

namespace Multimedia
{

/**
 * Discovers all MediaRenderer devices on the network and handles the connect and disconnect that
 * devices on the network.
 */
class BLABBYMULTIMEDIA_EXPORT RendererProvider : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates an instance of the RendererProvider
     * @param The @UPnPAV::IServiceProvider interface that is used to find the devices on the network.
     * @param The @UPnPAV::MediaRendererFactory creates @UPnPAV::MediaRenderer instances.
     */
    RendererProvider(std::unique_ptr<UPnPAV::IServiceProvider> serviceProider =
                         UPnPAV::ServiceProviderFactory{}.createServiceProvider(QString("")),
                     UPnPAV::MediaRendererFactory mrendererFab = UPnPAV::MediaRendererFactory{});
    /**
     * Default destructor
     */
    ~RendererProvider() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(RendererProvider)

    /**
     * Starts an MediaRenderer discovery
     * All connected MediaRenderer are reported with the @ref Multimedia::RendererProvider::rendererConnected signal.
     */
    void discover();

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Multimedia::RendererProvider dicovers a new @ref MultiMedia::Renderer
     * appears on the network.
     */
    void rendererConnected(std::shared_ptr<Multimedia::Renderer> const &renderer);

    /**
     * This signal is emitted when the @ref Multimedia::RendererProvider detects that a @ref Multimedia::Renderer
     * disapears on the Network.
     */
    void rendererDisconnected(std::shared_ptr<Multimedia::Renderer> const &renderer);

private Q_SLOTS:
    void onRendererDiscovered(QString const &usn) noexcept;
    void onRendererDisconnected(QString const &usn) noexcept;

private:
    std::unique_ptr<UPnPAV::IServiceProvider> mSp;
    UPnPAV::MediaRendererFactory mRendererFab;
    QHash<QString, std::shared_ptr<Renderer>> mRenderers;
};
} // namespace Multimedia
