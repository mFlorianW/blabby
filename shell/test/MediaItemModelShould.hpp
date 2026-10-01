// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace Shell
{
class MediaItemModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    Q_DISABLE_COPY_MOVE(MediaItemModelShould)
    ~MediaItemModelShould() override;
private Q_SLOTS:
    void give_correct_amount_of_items();
    void give_the_correct_display_roles();
    void give_the_correct_title_for_valid_index_data();
    void give_the_correct_title_for_valid_index();
    void navigate_when_a_container_item_is_activated();
    void update_the_media_items_when_navigation_is_finished();
    void request_to_play_only_the_activated_playable();
    void give_the_item_type_of_the_item();
    void tell_whether_a_media_source_is_set();
    void stop_following_the_previous_media_source_when_the_media_source_changes();
    void navigate_the_back_the_active_media_source();
    void give_the_artwork_url_and_the_secondary_text_of_the_item();
    void give_the_name_and_icon_url_for_the_active_media_source();
    void be_busy_until_the_items_of_the_opened_container_arrive();
    void ignore_activations_while_busy();
    void ignore_navigating_back_while_busy();
    void be_at_the_root_without_a_container_title_until_a_container_is_opened();
    void give_the_title_of_the_current_container();
    void return_to_the_parent_container_title_when_navigating_back();
    void ignore_navigating_back_at_the_root();
    void start_at_the_root_when_the_media_source_changes();
    void keep_the_current_container_when_opening_a_container_fails();
    void keep_the_container_below_the_root_when_opening_a_container_fails();
    void keep_the_current_container_when_navigating_back_fails();
    void fetch_more_items_while_the_media_source_can_load_more();
    void not_fetch_more_without_more_items();
    void not_fetch_more_while_more_items_are_loading();
    void not_fetch_more_while_busy();
    void keep_the_items_and_report_when_fetching_more_fails();
    void not_fetch_more_after_fetching_more_failed();
    void retry_fetching_more_after_it_failed();
    void ignore_retrying_when_fetching_more_did_not_fail();
    void clear_the_failure_when_another_container_is_opened();
    void clear_the_failure_when_the_media_source_changes();
    void stop_fetching_more_when_a_container_is_opened();
    void report_the_dropped_page_as_failed_when_opening_a_container_fails();
};

} // namespace Shell
