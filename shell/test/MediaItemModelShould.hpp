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
    void do_nothing_when_a_playable_item_is_activated();
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
};

} // namespace Shell
