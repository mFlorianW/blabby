// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Source.hpp"
#include <QHash>
#include <QObject>
#include <map>
#include <memory>

namespace Shell
{

/**
 * The @ref Shell::FolderArtwork gives a folder, a Container without Artwork and without an album, the Artwork of the
 * first Playable on its first page. A folder is browsed once, when its Artwork is first asked for; a folder without a
 * Playable on its first page, e.g. one holding only further folders, has no Artwork.
 */
class FolderArtwork final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(FolderArtwork)
public:
    FolderArtwork();
    ~FolderArtwork() override;

    /**
     * Sets the Source of the folders, forgetting the Artwork of the previous one.
     * @param source The Source, nullptr for none.
     */
    void setSource(std::shared_ptr<Multimedia::Source> source);

    /**
     * Gives the URL of the Artwork of a folder. While it's unknown the folder is browsed, @ref artworkFound tells when
     * it's known.
     * @param path The path of the folder.
     * @return The URL of the Artwork, empty while it's unknown or when the folder has no Artwork.
     */
    QString artworkUrl(QString const& path);

Q_SIGNALS:
    /**
     * This signal is emitted when a folder turned out to have Artwork.
     * @param path The path of the folder.
     */
    void artworkFound(QString const& path);

private:
    void onPageFinished(QString const& path);

    std::shared_ptr<Multimedia::Source> mSource;
    QHash<QString, QString> mArtworkUrls;
    std::map<QString, std::unique_ptr<Multimedia::PendingPage>> mPendingPages;
};

} // namespace Shell
