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
#ifndef MEDIASERVERMODELSHOULD_H
#define MEDIASERVERMODELSHOULD_H

#include <QObject>

namespace MediaServerPlugin
{

class MediaServerModelShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerModelShould)
public:
    MediaServerModelShould();

private Q_SLOTS:
    void give_correct_rolename();
    void give_mediaserver_name_on_request_with_valid_index();
    void give_mediaserver_icon_url_on_request_with_valid_index();
    void give_default_qvariant_for_a_to_small_index();
    void give_default_qvariant_for_a_to_big_index();
};

} // namespace MediaServerPlugin

#endif // MEDIASERVERMODELSHOULD_H
