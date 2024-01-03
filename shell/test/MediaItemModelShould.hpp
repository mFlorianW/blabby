// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
    void emit_playRequest_when_a_playable_item_is_activated();
    void navigate_the_back_the_active_media_source();
    void give_the_default_icon_url_when_the_media_item_has_no_icon();
    void give_the_name_and_icon_url_for_the_active_media_source();
};

} // namespace Shell
