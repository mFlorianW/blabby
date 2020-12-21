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
#include "ServerItemModelShould.h"
#include "MediaServerObject.h"
#include "ServerItemModel.h"
#include <QSignalSpy>
#include <QTest>

namespace MediaServerPlugin
{

ServerItemModelShould::ServerItemModelShould(QObject *parent)
    : QObject(parent)
{
}

void ServerItemModelShould::give_correct_role_name()
{
    ServerItemModel model;
    const auto expectedRoles = QHash<qint32, QByteArray>{ std::make_pair(Qt::UserRole + 1, "itemClass"),
                                                          std::make_pair(Qt::UserRole + 2, "itemName") };

    const auto roles = model.roleNames();

    QCOMPARE(roles.size(), expectedRoles.size());
    QCOMPARE(roles, expectedRoles);
}

void ServerItemModelShould::give_the_correct_number_of_items_in_the_model()
{
    ServerItemModel model;
    const auto expectedResult = 1;
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "storageFolder" };
    model.insertMediaServerObject(mediaObject);

    QCOMPARE(model.rowCount(QModelIndex{}), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_folder_for_inserted_media_object()
{
    ServerItemModel model;
    const auto expectedResult = ServerItemModel::ItemFolder;
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "storageFolder" };
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemClass);

    QCOMPARE(result.value<ServerItemModel::ItemType>(), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_file_for_inserted_media_object()
{
    ServerItemModel model;
    const auto expectedResult = ServerItemModel::ItemFile;
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemClass);

    QCOMPARE(result.value<ServerItemModel::ItemType>(), expectedResult);
}

void ServerItemModelShould::give_the_correct_itemType_for_multiple_media_objects()
{
    ServerItemModel model;
    const auto expectedResult0 = ServerItemModel::ItemFile;
    const auto expectedResult1 = ServerItemModel::ItemFolder;
    const auto mediaObjectFile = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
    const auto mediaObjectFolder = UPnPAV::MediaServerObject{ "", "", "testFolder", "storageFolder" };
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
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(0), ServerItemModel::ItemName);

    QCOMPARE(result.value<QString>(), expectedResult);
}

void ServerItemModelShould::give_empty_variant_for_to_small_index()
{
    ServerItemModel model;
    const auto expectedResult = QVariant{};
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(-1), ServerItemModel::ItemClass);

    QCOMPARE(result, expectedResult);
}

void ServerItemModelShould::give_empty_variant_for_to_big_index()
{
    ServerItemModel model;
    const auto expectedResult = QVariant{};
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
    model.insertMediaServerObject(mediaObject);

    const auto result = model.data(model.index(3), ServerItemModel::ItemClass);

    QCOMPARE(result, expectedResult);
}

void ServerItemModelShould::emit_rowsAboutToInsert_and_rowsInserted_when_a_media_object_added()
{
    ServerItemModel model;
    const auto mediaObject = UPnPAV::MediaServerObject{ "", "", "testFolder", "audioItem" };
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

} // namespace MediaServerPlugin

QTEST_MAIN(MediaServerPlugin::ServerItemModelShould);