// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
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
    UPnPAV::Doubles::MediaRendererDouble* mUpnpRenderer;
    std::shared_ptr<MediaPlayer> mPlayer;
private Q_SLOTS:
    void init();

    void start_stop_and_resume_playback_on_play_request();
    void forwared_the_volume_of_the_used_renderer();
    void forwared_set_the_volume_to_the_used_renderer();
};
} // namespace Shell
