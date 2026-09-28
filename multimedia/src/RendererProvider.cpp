// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RendererProvider.hpp"
#include "InvalidDeviceDescription.hpp"
#include "private/LoggingCategories.hpp"

namespace Multimedia
{

RendererProvider::RendererProvider(std::unique_ptr<UPnPAV::IServiceProvider> serviceProvider,
                                   std::unique_ptr<UPnPAV::MediaRendererFactory> rendererFab)
    : mSp{std::move(serviceProvider)}
    , mRendererFab{std::move(rendererFab)}
{
    Q_ASSERT(mSp != nullptr);
    Q_ASSERT(mRendererFab != nullptr);
    mSp->setSearchTarget(QStringLiteral("urn:schemas-upnp-org:device:MediaRenderer:1"));

    connect(mSp.get(), &UPnPAV::IServiceProvider::serviceConnected, this, &RendererProvider::onRendererDiscovered);
    connect(mSp.get(), &UPnPAV::IServiceProvider::serviceDisconnected, this, &RendererProvider::onRendererDisconnected);
    connect(mSp.get(), &UPnPAV::IServiceProvider::searchFinished, this, &RendererProvider::discoveryFinished);
}

RendererProvider::~RendererProvider() = default;

void RendererProvider::discover()
{
    mSp->startSearch();
}

void RendererProvider::onRendererDiscovered(QString const& usn) noexcept
{
    try {
        auto const desc = mSp->rootDeviceDescription(usn);
        auto upnpRenderer = mRendererFab->create(desc);
        auto renderer = std::make_shared<Renderer>(std::move(upnpRenderer));
        mRenderers.insert(usn, renderer);
        Q_EMIT rendererConnected(renderer);
    } catch (UPnPAV::InvalidDeviceDescription const& exception) {
        qCCritical(mmRenderer) << "Failed to create Renderer. Error:" << exception.what();
    }
}

void RendererProvider::onRendererDisconnected(QString const& usn) noexcept
{
    if (mRenderers.contains(usn)) {
        auto const renderer = mRenderers.value(usn);
        mRenderers.remove(usn);
        Q_EMIT rendererDisconnected(renderer);
    }
}

} // namespace Multimedia
