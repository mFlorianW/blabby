// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "IServiceProvider.hpp"
#include "MediaRenderer.hpp"
#include "Renderer.hpp"
#include "RendererStore.hpp"
#include "ServiceProvider.hpp"
#include "blabbymultimedia_export.h"
#include <QHash>
#include <QObject>

namespace Multimedia
{

/**
 * Discovers all MediaRenderer devices on the network and handles the connect and disconnect that
 * devices on the network.
 * Every @ref Multimedia::Renderer that was seen is remembered in the @ref Multimedia::RendererStore, is Offline
 * after a restart until it's discovered again and stays known as Offline when it leaves the network.
 * A @ref Multimedia::Renderer whose device doesn't answer anymore goes Offline and is Online again on its next
 * announcement.
 */
class BLABBYMULTIMEDIA_EXPORT RendererProvider : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates an instance of the RendererProvider
     * The remembered @ref Multimedia::Renderer are loaded from the store as Offline Renderers.
     * @param store The @ref Multimedia::RendererStore that keeps the remembered Renderers.
     * @param serviceProvider The @UPnPAV::IServiceProvider interface that is used to find the devices on the network.
     * @param rendererFab The @UPnPAV::MediaRendererFactory creates @UPnPAV::MediaRenderer instances.
     */
    explicit RendererProvider(
        std::shared_ptr<RendererStore> store,
        std::unique_ptr<UPnPAV::IServiceProvider> serviceProvider =
            UPnPAV::ServiceProviderFactory{}.createServiceProvider(QString("")),
        std::unique_ptr<UPnPAV::MediaRendererFactory> rendererFab = std::make_unique<UPnPAV::MediaRendererFactory>());
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
     * The end of the discovery is reported with the @ref Multimedia::RendererProvider::discoveryFinished signal.
     */
    void discover();

    /**
     * Gives all known @ref Multimedia::Renderer, the remembered and the discovered ones, Online or Offline.
     * @return The known Renderers.
     */
    QList<std::shared_ptr<Renderer>> const& renderers() const noexcept;

    /**
     * Forgets an Offline @ref Multimedia::Renderer: it's removed from the known and the remembered Renderers.
     * When it's discovered again it's remembered again as a newly seen @ref Multimedia::Renderer.
     * Online Renderers can't be forgotten.
     * @param renderer The @ref Multimedia::Renderer to forget.
     * @return True when the @ref Multimedia::Renderer was forgotten, otherwise false.
     */
    bool forget(std::shared_ptr<Renderer> const& renderer) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Multimedia::RendererProvider discovers a @ref MultiMedia::Renderer
     * on the network. A known @ref Multimedia::Renderer is recognised by its identity and is Online again when
     * the signal is emitted, an unknown one is added to the known Renderers.
     */
    void rendererConnected(std::shared_ptr<Multimedia::Renderer> const& renderer);

    /**
     * This signal is emitted when the @ref Multimedia::RendererProvider detects that a @ref Multimedia::Renderer
     * disapears on the Network. The @ref Multimedia::Renderer is Offline and stays known.
     */
    void rendererDisconnected(std::shared_ptr<Multimedia::Renderer> const& renderer);

    /**
     * This signal is emitted when the MediaRenderer on the network had the time to answer a discovery.
     */
    void discoveryFinished();

private Q_SLOTS:
    void onRendererDiscovered(QString const& usn) noexcept;
    void onRendererDisconnected(QString const& usn) noexcept;

private:
    std::shared_ptr<Renderer> knownRenderer(QString const& identity) const noexcept;
    void addRenderer(std::shared_ptr<Renderer> const& renderer) noexcept;
    void onRendererAvailabilityChanged(Renderer const* renderer) noexcept;
    void saveKnownRenderers() noexcept;

    std::shared_ptr<RendererStore> mStore;
    std::unique_ptr<UPnPAV::IServiceProvider> mSp;
    std::unique_ptr<UPnPAV::MediaRendererFactory> mRendererFab;
    QList<std::shared_ptr<Renderer>> mRenderers;
    QHash<QString, std::shared_ptr<Renderer>> mOnlineRenderers;
};
} // namespace Multimedia
