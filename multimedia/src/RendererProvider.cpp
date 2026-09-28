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

RendererProvider::RendererProvider(std::shared_ptr<RendererStore> store,
                                   std::unique_ptr<UPnPAV::IServiceProvider> serviceProvider,
                                   std::unique_ptr<UPnPAV::MediaRendererFactory> rendererFab)
    : mStore{std::move(store)}
    , mSp{std::move(serviceProvider)}
    , mRendererFab{std::move(rendererFab)}
{
    Q_ASSERT(mStore != nullptr);
    Q_ASSERT(mSp != nullptr);
    Q_ASSERT(mRendererFab != nullptr);

    auto const rememberedRenderers = mStore->load();
    for (auto const& remembered : rememberedRenderers) {
        if (knownRenderer(remembered.identity) == nullptr) {
            mRenderers.append(std::make_shared<Renderer>(remembered));
        }
    }

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

QList<std::shared_ptr<Renderer>> const& RendererProvider::renderers() const noexcept
{
    return mRenderers;
}

void RendererProvider::onRendererDiscovered(QString const& usn) noexcept
{
    try {
        auto const desc = mSp->rootDeviceDescription(usn);
        auto upnpRenderer = mRendererFab->create(desc);
        auto renderer = knownRenderer(upnpRenderer->udn());
        auto changed = true;
        if (renderer != nullptr) {
            auto const previous = renderer->remembered();
            renderer->goOnline(std::move(upnpRenderer));
            changed = renderer->remembered() != previous;
        } else {
            renderer = std::make_shared<Renderer>(std::move(upnpRenderer));
            mRenderers.append(renderer);
        }
        mOnlineRenderers.insert(usn, renderer);
        if (changed) {
            saveKnownRenderers();
        }
        Q_EMIT rendererConnected(renderer);
    } catch (UPnPAV::InvalidDeviceDescription const& exception) {
        qCCritical(mmRenderer) << "Failed to create Renderer. Error:" << exception.what();
    }
}

void RendererProvider::onRendererDisconnected(QString const& usn) noexcept
{
    auto const renderer = mOnlineRenderers.take(usn);
    if (renderer != nullptr) {
        renderer->goOffline();
        Q_EMIT rendererDisconnected(renderer);
    }
}

std::shared_ptr<Renderer> RendererProvider::knownRenderer(QString const& identity) const noexcept
{
    auto const iter = std::find_if(mRenderers.cbegin(), mRenderers.cend(), [&identity](auto const& renderer) {
        return renderer->identity() == identity;
    });
    return iter != mRenderers.cend() ? *iter : nullptr;
}

void RendererProvider::saveKnownRenderers() noexcept
{
    auto remembered = QList<RememberedRenderer>{};
    remembered.reserve(mRenderers.size());
    for (auto const& renderer : std::as_const(mRenderers)) {
        remembered.append(renderer->remembered());
    }
    mStore->save(remembered);
}

} // namespace Multimedia
