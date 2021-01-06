/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
};

} // namespace MediaServer::Plugin

#endif // SERVERITEMCONTROLLERSHOULD_H
