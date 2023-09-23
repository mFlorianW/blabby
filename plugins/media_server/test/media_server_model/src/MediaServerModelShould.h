// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERMODELSHOULD_H
#define MEDIASERVERMODELSHOULD_H

#include <QObject>

namespace MediaServer::Plugin
{

class MediaServerModelShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerModelShould)
public:
    MediaServerModelShould();

private Q_SLOTS:
    void give_correct_rolename();
    void give_mediaserver_name_on_request_with_valid_index();
    void give_mediaserver_icon_url_on_request_with_valid_index();
    void give_default_qvariant_for_a_to_small_index();
    void give_default_qvariant_for_a_to_big_index();
    void do_not_add_media_server_twice();
    void remove_inserted_media_server_from_model();
    void give_media_server_for_name();
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERMODELSHOULD_H
