// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace Provider::MediaServer
{

class SourceShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~SourceShould() override;
    Q_DISABLE_COPY_MOVE(SourceShould);
private Q_SLOTS:
    void give_the_name_of_the_media_server();
    void give_the_icon_of_the_media_server();
    void request_root_media_items_on_init();
    void give_root_media_items_on_init();
    void send_correct_request_on_navigation();
    void request_root_media_items_on_navigation();
    void give_a_default_icon_when_no_icon_is_set();
    void classify_objects_by_their_class_data();
    void classify_objects_by_their_class();
    void request_the_album_art_the_artist_and_the_creator();
    void map_the_album_art_and_the_artist_to_the_item_data();
    void map_the_album_art_and_the_artist_to_the_item();
    void report_a_failed_browse_and_keep_the_items();
};

} // namespace Provider::MediaServer
