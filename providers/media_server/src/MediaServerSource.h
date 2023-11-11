// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include "MediaServer.h"
#include "MediaSource.h"
#include "blabbymediaserverprovider_export.h"

namespace Provider
{

class BLABBYMEDIASERVERPROVIDER_EXPORT MediaServerSource final : public Multimedia::MediaSource
{
public:
    /**
     * Creates an instance of MediaServerSource
     * @mediaServer The UPnP MediaServer that shall be controlled by this source.
     */
    MediaServerSource(std::unique_ptr<UPnPAV::MediaServer> mediaServer);

    /**
     * The default desctructor
     */
    ~MediaServerSource() override;

    /**
     * Disable copy or move
     */
    Q_DISABLE_COPY_MOVE(MediaServerSource)

    /**
     * @copydoc Multimedia::MediaSource::navigateTo(QString)
     */
    void navigateTo(QString const &path) noexcept override;

private Q_SLOTS:
    void onBrowseRequestFinished() noexcept;

private:
    void navigate(QString const &path) noexcept;
    struct BrowseRequest
    {
        QSharedPointer<UPnPAV::PendingSoapCall> mRequest;
        QString mPath;
    };

    std::unique_ptr<UPnPAV::MediaServer> mServer;
    BrowseRequest mBrowseRequest;
};
} // namespace Provider
