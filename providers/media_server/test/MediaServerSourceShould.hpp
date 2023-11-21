// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace Provider
{

class MediaServerSourceShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaServerSourceShould() override;
    Q_DISABLE_COPY_MOVE(MediaServerSourceShould);
private Q_SLOTS:
    void give_the_name_of_the_media_server();
    void give_the_icon_of_the_media_server();
    void request_root_media_items_on_init();
    void give_root_media_items_on_init();
    void send_correct_request_on_navigation();
    void request_root_media_items_on_navigation();
};

} // namespace Provider
