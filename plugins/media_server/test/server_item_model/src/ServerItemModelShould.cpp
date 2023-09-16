// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ServerItemModelShould.h"
#include "MediaServerObject.h"
#include "ServerItemModel.h"
#include <QSignalSpy>
#include <QTest>

namespace MediaServer::Plugin
{

ServerItemModelShould::ServerItemModelShould(QObject *parent)
    : QObject(parent)
{
}

void ServerItemModelShould::give_correct_role_name()
{
    ServerItemModel model;
    const auto expectedRoles = QHash<qint32, QByteArray>{std::make_pair(Qt::UserRole + 1, "itemClass"),
                                                         std::make_pair(Qt::UserRole + 2, "itemName"),
                                                         std::make_pair(Qt::UserRole + 3, "itemId")};

    const auto roles = model.roleNames();

    QCOMPARE(roles.size(), expectedRoles.size());
    QCOMPARE(roles, expectedRoles);
}

void ServerItemModelShould::give_the_correct_number_of_items_in_the_model()
{
    ServerItemModel model;
    const auto expectedResult = 1;
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "storageFolder"};
    model.insertMediaServerObject(mediaObject);

    QCOMPARE(model.rowCount(QModelIndex{}), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_folder_for_inserted_media_object()
{
    ServerItemModel model;
    const auto expectedResult = ServerItemModel::ItemFolder;
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "storageFolder"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemClass);

    QCOMPARE(result.value<ServerItemModel::ItemType>(), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_file_for_inserted_media_object()
{
    ServerItemModel model;
    const auto expectedResult = ServerItemModel::ItemFile;
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemClass);

    QCOMPARE(result.value<ServerItemModel::ItemType>(), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_for_multiple_media_objects()
{
    ServerItemModel model;
    const auto expectedResult0 = ServerItemModel::ItemFile;
    const auto expectedResult1 = ServerItemModel::ItemFolder;
    const auto mediaObjectFile = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    const auto mediaObjectFolder = UPnPAV::MediaServerObject{"", "", "testFolder", "storageFolder"};
    model.insertMediaServerObject(mediaObjectFile);
    model.insertMediaServerObject(mediaObjectFolder);

    const auto result = model.data(model.index(0), ServerItemModel::ItemClass);
    QCOMPARE(expectedResult0, result.value<ServerItemModel::ItemType>());

    const auto result1 = model.data(model.index(1), ServerItemModel::ItemClass);
    QCOMPARE(result1.value<ServerItemModel::ItemType>(), expectedResult1);
}

void ServerItemModelShould::give_the_correct_itemType_name_for_media_object()
{
    ServerItemModel model;
    const auto expectedResult = "testFolder";
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemName);

    QCOMPARE(result.value<QString>(), expectedResult);
}

void ServerItemModelShould::give_empty_variant_for_to_small_index()
{
    ServerItemModel model;
    const auto expectedResult = QVariant{};
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(-1), ServerItemModel::ItemClass);

    QCOMPARE(result, expectedResult);
}

void ServerItemModelShould::give_empty_variant_for_to_big_index()
{
    ServerItemModel model;
    const auto expectedResult = QVariant{};
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(3), ServerItemModel::ItemClass);

    QCOMPARE(result, expectedResult);
}

void ServerItemModelShould::emit_rowsAboutToInsert_and_rowsInserted_when_a_media_object_added()
{
    ServerItemModel model;
    const auto mediaObject = UPnPAV::MediaServerObject{"", "", "testFolder", "audioItem"};
    QSignalSpy rowsAboutToInsertSpy(&model, &ServerItemModel::rowsAboutToBeInserted);
    QSignalSpy rowsInsertedSpy(&model, &ServerItemModel::rowsInserted);
    model.insertMediaServerObject(mediaObject);

    QCOMPARE(rowsAboutToInsertSpy.count(), 1);
    const auto argsRowsAboutToInsert = rowsAboutToInsertSpy.takeFirst();
    QCOMPARE(argsRowsAboutToInsert.length(), 3);
    QCOMPARE(argsRowsAboutToInsert.at(0).value<QModelIndex>(), model.index(-1));
    QCOMPARE(argsRowsAboutToInsert.at(1).value<int>(), 0);
    QCOMPARE(argsRowsAboutToInsert.at(2).value<int>(), 0);

    QCOMPARE(rowsInsertedSpy.count(), 1);
    const auto argsInserted = rowsInsertedSpy.takeFirst();
    QCOMPARE(argsInserted.length(), 3);
    QCOMPARE(argsInserted.at(0).value<QModelIndex>(), model.index(-1));
    QCOMPARE(argsInserted.at(1).value<int>(), 0);
    QCOMPARE(argsInserted.at(2).value<int>(), 0);
}

void ServerItemModelShould::give_the_item_id_for_inserted_media_object()
{
    ServerItemModel model;
    const auto expectedResult = "0";
    const auto mediaObject = UPnPAV::MediaServerObject{"0", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemId);

    QCOMPARE(result.value<QString>(), expectedResult);
}

void ServerItemModelShould::clear_all_objects()
{
    ServerItemModel model;
    const auto mediaObject = UPnPAV::MediaServerObject{"0", "", "testFolder", "audioItem"};
    model.insertMediaServerObject(mediaObject);
    QSignalSpy modelAboutToResetSpy{&model, &ServerItemModel::modelAboutToBeReset};
    QSignalSpy modelResetSpy{&model, &ServerItemModel::modelReset};

    QCOMPARE(model.rowCount(QModelIndex{}), 1);

    model.clearMediaServerObjects();

    QCOMPARE(modelAboutToResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(model.rowCount(QModelIndex{}), 0);
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::ServerItemModelShould);
