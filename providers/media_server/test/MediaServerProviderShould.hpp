// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
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
    void describe_a_rescan_network_action_under_media_servers();
    void search_the_network_again_on_rescan();
    void ignore_unknown_actions();
    void report_servers_found_by_a_rescan_as_added();
    void report_servers_missing_after_a_rescan_as_removed();
    void remove_servers_only_at_the_end_of_a_rescan();
};

}; // namespace Provider::MediaServer
