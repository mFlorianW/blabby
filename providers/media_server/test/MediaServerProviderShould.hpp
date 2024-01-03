// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QObject>

namespace Provider::MediaServer
{

class ProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ProviderShould() override;
    Q_DISABLE_COPY_MOVE(ProviderShould);
private Q_SLOTS:
    void send_find_request_for_media_server_on_init();
    void notify_when_a_server_appears();
    void notify_when_a_server_disappears();
};

}; // namespace Provider::MediaServer
