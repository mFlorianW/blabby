// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
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
     * @copydoc Multimedia::MediaSource::navigateTo(QString)
     */
    void navigateTo(QString const &path) noexcept override;

private Q_SLOTS:
    void onBrowseRequestFinished() noexcept;

private:
    void navigate(QString const &path) noexcept;
    struct BrowseRequest
    {
        std::unique_ptr<UPnPAV::PendingSoapCall> mRequest;
        QString mPath;
    };

    std::unique_ptr<UPnPAV::MediaServer> mServer;
    BrowseRequest mBrowseRequest;
};
} // namespace Provider::MediaServer
