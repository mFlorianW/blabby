// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SERVERITEMVIEWSHOULD_H
#define SERVERITEMVIEWSHOULD_H

#include <QObject>

namespace MediaServer::Plugin
{

class ServerItemViewShould : public QObject
{
    Q_OBJECT
public:
    ServerItemViewShould();

private Q_SLOTS:
    void on_inserted_media_server_request_root_folder();
    void on_valid_result_received_insert_media_objects_into_server_item_model();
    void request_specific_folder_on_media_server();
    void on_valid_response_of_specific_request_fill_objects_into_server_item_model();
    void go_to_previous_folder();
    void go_to_back_to_root_folder();
};

} // namespace MediaServer::Plugin

#endif // SERVERITEMCONTROLLERSHOULD_H
