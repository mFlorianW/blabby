// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "FolderArtwork.hpp"
#include "ArtworkUrl.hpp"
#include <algorithm>

namespace Shell
{

FolderArtwork::FolderArtwork() = default;
FolderArtwork::~FolderArtwork() = default;

void FolderArtwork::setSource(std::shared_ptr<Multimedia::Source> source)
{
    mSource = std::move(source);
    mArtworkUrls.clear();
    mPendingPages.clear();
}

QString FolderArtwork::artworkUrl(QString const& path)
{
    if (auto const known = mArtworkUrls.constFind(path); known != mArtworkUrls.cend()) {
        return known.value();
    }
    if (mSource == nullptr or mPendingPages.contains(path)) {
        return {};
    }

    auto& page = mPendingPages[path] = mSource->browsePage(path, 0);
    if (page->isFinished()) {
        // Handled later, the caller is still asking for the Artwork.
        QMetaObject::invokeMethod(
            this,
            [this, path] {
                onPageFinished(path);
            },
            Qt::QueuedConnection);
    } else {
        connect(page.get(), &Multimedia::PendingPage::finished, this, [this, path] {
            onPageFinished(path);
        });
    }
    return {};
}

void FolderArtwork::onPageFinished(QString const& path)
{
    auto const pending = mPendingPages.find(path);
    if (pending == mPendingPages.end()) {
        return;
    }
    // The page is the sender, it's deleted once its signal is handled.
    auto* const page = pending->second.release();
    page->deleteLater();
    mPendingPages.erase(pending);

    auto const& items = page->items();
    auto const playable = std::ranges::find(items, Multimedia::ItemType::Playable, &Multimedia::Item::type);
    auto const url = playable != items.cend() ? Shell::artworkUrl(*playable) : QString{};
    mArtworkUrls.insert(path, url);
    if (not url.isEmpty()) {
        Q_EMIT artworkFound(path);
    }
}

} // namespace Shell
