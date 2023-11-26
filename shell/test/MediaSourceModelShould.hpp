// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace Shell
{
class MediaSourceModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaSourceModelShould() override;
    Q_DISABLE_COPY_MOVE(MediaSourceModelShould)
private Q_SLOTS:
    void load_the_provider_and_report_amount_of_media_sources();
    void give_the_correct_role_names_for_the_ui();
    void give_the_name_of_provider_for_valid_index();
    void give_the_iconurl_of_provider_for_valid_index();
    void handle_new_connected_sources();
    void handle_disconnected_sources();
};

} // namespace Shell
