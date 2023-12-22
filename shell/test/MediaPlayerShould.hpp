// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "MediaPlayer.hpp"
#include "MediaRendererDouble.hpp"
#include <QObject>

namespace Shell
{
class MediaPlayerShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaPlayerShould() override;
    Q_DISABLE_COPY_MOVE(MediaPlayerShould)
private:
    UPnPAV::Doubles::MediaRendererDouble *mUpnpRenderer;
    std::shared_ptr<MediaPlayer> mPlayer;
private Q_SLOTS:
    /**
     * Setups for every test a @ref Shell::MediaPlayer instance.
     * In the tests the @ref Shell::MediaPlayer can be asccessed with mPlayer.
     * The simulated @ref UPnPAV::MediaRenderer is accessable with mUpnpRenderer.
     */
    void init();

    /**
     * Tests that a passed playable MediaItem to the @ref Shell::MediaPlayer is correctly forwarded
     * and the playback start is correctly reported.
     */
    void start_playback_on_play_request();
};
} // namespace Shell
