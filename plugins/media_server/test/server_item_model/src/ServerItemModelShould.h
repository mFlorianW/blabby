// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
