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
#include <QObject>

namespace MediaServer::Plugin
{
class MainWindowShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindowShould)
public:
    MainWindowShould();
    ~MainWindowShould();

private Q_SLOTS:
    void on_mediaserver_connected_create_media_server_and_put_in_the_model();
    void handle_every_connected_signal_of_media_server_once();
    void handle_disconnected_media_server();
    void set_active_media_server();
};

} // namespace MediaServer::Plugin
