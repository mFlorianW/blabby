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
#ifndef SERVERITEMMODELSHOULD_H
#define SERVERITEMMODELSHOULD_H

#include <QObject>

namespace MediaServer::Plugin
{

class ServerItemModelShould : public QObject
{
    Q_OBJECT
public:
    explicit ServerItemModelShould(QObject *parent = nullptr);

private Q_SLOTS:
    void give_correct_role_name();
    void give_the_correct_number_of_items_in_the_model();
    void give_the_correct_itemType_folder_for_inserted_media_object();
    void give_the_correct_itemType_file_for_inserted_media_object();
    void give_the_correct_itemType_for_multiple_media_objects();
    void give_the_correct_itemType_name_for_media_object();
    void give_empty_variant_for_to_small_index();
    void give_empty_variant_for_to_big_index();
    void emit_rowsAboutToInsert_and_rowsInserted_when_a_media_object_added();
    void give_the_item_id_for_inserted_media_object();
    void clear_all_objects();
};

} // namespace MediaServer::Plugin
#endif // SERVERITEMMODELSHOULD_H
