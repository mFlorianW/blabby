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
    void request_the_album_and_the_duration();
    void map_the_album_and_the_duration_of_the_played_resource_to_the_item();
    void report_a_failed_browse_and_keep_the_items();
    void request_the_first_page_of_a_container();
    void give_the_total_item_count_of_the_container();
    void request_the_next_page_of_the_current_container();
    void append_the_items_of_the_next_page();
    void take_the_latest_total_item_count();
    void end_loading_on_a_page_without_items();
    void report_a_failed_page_and_keep_the_items();
    void not_load_more_when_every_item_is_loaded();
    void not_load_more_while_browsing();
    void navigate_instead_of_loading_more_when_navigating_meanwhile();
    void request_at_least_the_minimum_item_count_when_navigating();
    void browse_until_the_minimum_item_count_is_loaded();
    void finish_navigating_when_the_container_has_fewer_items_than_the_minimum();
    void finish_navigating_on_a_page_without_items();
    void keep_the_items_when_a_later_page_of_a_navigation_fails();
    void request_a_page_of_a_container();
    void give_the_items_of_a_page_without_navigating();
    void report_a_failed_page_of_a_container();
};

} // namespace Provider::MediaServer
