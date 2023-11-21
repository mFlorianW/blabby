// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QObject>

namespace Provider
{

class MediaServerProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaServerProviderShould() override;
    Q_DISABLE_COPY_MOVE(MediaServerProviderShould);
private Q_SLOTS:
    void send_find_request_for_media_server_on_init();
    void notify_when_a_server_appears();
    void notify_when_a_server_disappears();
};

}; // namespace Provider
