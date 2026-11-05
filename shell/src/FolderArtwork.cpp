// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "FolderArtwork.hpp"
#include "ArtworkUrl.hpp"
#include <QRandomGenerator>
#include <algorithm>

namespace Shell
{

namespace
{

// The folder whose Artwork is asked for and up to two levels of its subfolders are browsed.
constexpr auto maximumDepth = 2;

/**
 * Gives one of the Items, chosen at random.
 */
Multimedia::Item const& anyOf(QList<Multimedia::Item> const& items)
{
    return items.at(static_cast<qsizetype>(QRandomGenerator::global()->bounded(static_cast<qint64>(items.size()))));
}

} // namespace

FolderArtwork::FolderArtwork() = default;
FolderArtwork::~FolderArtwork() = default;

void FolderArtwork::setSource(std::shared_ptr<Multimedia::Source> source)
{
    mSource = std::move(source);
    mArtworkUrls.clear();
    mPendingFolders.clear();
    mWaitingFolders.clear();
    mFoldersWaitingForSubfolders.clear();
}

QString FolderArtwork::artworkUrl(QString const& path)
{
    if (auto const known = mArtworkUrls.constFind(path); known != mArtworkUrls.cend()) {
        return known.value();
    }
    if (mSource != nullptr and not mPendingFolders.contains(path) and not mFoldersWaitingForSubfolders.contains(path)) {
        browse(path, 0);
    }
    return {};
}

void FolderArtwork::browse(QString const& path, int depth)
{
    auto& folder = mPendingFolders[path] = PendingFolder{.page = mSource->browsePage(path, 0), .depth = depth};
    if (folder.page->isFinished()) {
        // Handled later, the caller is still asking for the Artwork.
        QMetaObject::invokeMethod(
            this,
            [this, path] {
                onPageFinished(path);
            },
            Qt::QueuedConnection);
    } else {
        connect(folder.page.get(), &Multimedia::PendingPage::finished, this, [this, path] {
            onPageFinished(path);
        });
    }
}

void FolderArtwork::onPageFinished(QString const& path)
{
    auto const pending = mPendingFolders.find(path);
    if (pending == mPendingFolders.end()) {
        return;
    }
    // The page is the sender, it's deleted once its signal is handled.
    auto* const page = pending->second.page.release();
    page->deleteLater();
    auto const depth = pending->second.depth;
    mPendingFolders.erase(pending);

    auto const& items = page->items();
    auto const playable = std::ranges::find(items, Multimedia::ItemType::Playable, &Multimedia::Item::type);
    if (playable != items.cend()) {
        resolve(path, Shell::artworkUrl(*playable));
        return;
    }

    auto subfolders = QList<Multimedia::Item>{};
    auto subfoldersWithArtwork = QList<Multimedia::Item>{};
    for (auto const& item : items) {
        if (item.type() == Multimedia::ItemType::Container) {
            subfolders.append(item);
            if (not Shell::artworkUrl(item).isEmpty()) {
                subfoldersWithArtwork.append(item);
            }
        }
    }

    if (not subfoldersWithArtwork.isEmpty()) {
        resolve(path, Shell::artworkUrl(anyOf(subfoldersWithArtwork)));
    } else if (subfolders.isEmpty() or depth >= maximumDepth) {
        resolve(path, {});
    } else if (auto const subfolder = anyOf(subfolders).path(); mArtworkUrls.contains(subfolder)) {
        resolve(path, mArtworkUrls.value(subfolder));
    } else {
        mWaitingFolders[subfolder].append(path);
        mFoldersWaitingForSubfolders.insert(path);
        if (not mPendingFolders.contains(subfolder)) {
            browse(subfolder, depth + 1);
        }
    }
}

void FolderArtwork::resolve(QString const& path, QString const& url)
{
    mArtworkUrls.insert(path, url);
    mFoldersWaitingForSubfolders.remove(path);
    if (not url.isEmpty()) {
        Q_EMIT artworkFound(path);
    }
    auto const waitingFolders = mWaitingFolders.take(path);
    for (auto const& waitingFolder : waitingFolders) {
        resolve(waitingFolder, url);
    }
}

} // namespace Shell
