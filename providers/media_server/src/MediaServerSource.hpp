// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include "MediaServer.hpp"
#include "Source.hpp"
#include "blabbymediaserverprovider_export.h"

namespace Provider::MediaServer
{

class BLABBYMEDIASERVERPROVIDER_EXPORT Source final : public Multimedia::Source
{
public:
    /**
     * Creates an instance of MediaServerSource
     * @mediaServer The UPnP MediaServer that shall be controlled by this source.
     */
    Source(std::unique_ptr<UPnPAV::MediaServer> mediaServer);

    /**
     * The default desctructor
     */
    ~Source() override;

    /**
     * Disable copy or move
     */
    Q_DISABLE_COPY_MOVE(Source)

    /**
     * @copydoc Multimedia::Source::loadMore()
     */
    void loadMore() noexcept override;

protected:
    /**
     * @copydoc Multimedia::Source::navigate(QString, qsizetype)
     */
    void navigate(QString const& path, qsizetype minimumItemCount) noexcept override;

private Q_SLOTS:
    void onBrowseRequestFinished() noexcept;

private:
    /**
     * What a Browse request loads: the first page of a Container to navigate to, or the next page of the current one.
     */
    enum class BrowseKind
    {
        Navigation,
        NextPage,
    };

    /**
     * A navigation in progress, its Items replace the current ones once at least the minimum number is loaded.
     */
    struct Navigation
    {
        QString mPath;
        qsizetype mMinimumItemCount{0};
        Multimedia::Items mItems;
    };

    struct BrowseRequest
    {
        std::unique_ptr<UPnPAV::PendingSoapCall> mRequest;
        BrowseKind mKind{BrowseKind::Navigation};
        bool mPending{false};
    };

    void browse(QString const& path, BrowseKind kind, qsizetype startingIndex, qsizetype requestedCount) noexcept;
    void finishNavigationPage(Multimedia::Items page, qsizetype totalMatches) noexcept;
    void finishNextPage(Multimedia::Items page, qsizetype totalMatches) noexcept;

    std::unique_ptr<UPnPAV::MediaServer> mServer;
    BrowseRequest mBrowseRequest;
    Navigation mNavigation;
    // The Container of the last finished navigation, whose next pages are loaded.
    QString mCurrentPath;
};
} // namespace Provider::MediaServer
