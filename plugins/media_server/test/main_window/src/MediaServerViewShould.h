// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERVIEWSHOULD_H
#define MEDIASERVERVIEWSHOULD_H
#include <QObject>

namespace MediaServer::Plugin
{
class MediaServerViewShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerViewShould)
public:
    MediaServerViewShould();
    ~MediaServerViewShould();

private Q_SLOTS:
    void on_mediaserver_connected_create_media_server_and_put_in_the_model();
    void handle_every_connected_signal_of_media_server_once();
    void handle_disconnected_media_server();
    void set_active_media_server();
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERVIEWSHOULD_H
