// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SERVERITEMMODEL_H
#define SERVERITEMMODEL_H

#include "MediaServerObject.h"
#include <QAbstractListModel>

namespace MediaServer::Plugin
{

class ServerItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServerItemModel)
public:
    enum RoleName
    {
        ItemClass = Qt::UserRole + 1,
        ItemName,
        ItemId
    };
    Q_ENUM(RoleName)

    enum ItemType
    {
        ItemFolder,
        ItemFile,
    };
    Q_ENUM(ItemType)

    ServerItemModel();
    ~ServerItemModel() override;

    void insertMediaServerObject(const UPnPAV::MediaServerObject &object);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void clearMediaServerObjects();

private:
    QVector<UPnPAV::MediaServerObject> mMediaServerObjects;
};

} // namespace MediaServer::Plugin

#endif // SERVERITEMMODEL_H
