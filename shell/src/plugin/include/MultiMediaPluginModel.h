// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MULTIMEDIAPLUGINMODEL_H
#define MULTIMEDIAPLUGINMODEL_H

#include <QAbstractListModel>
#include <memory>
#include <qqml.h>

namespace PluginCore
{
class MultimediaPlugin;
}

namespace Shell
{
class MultiMediaPluginModelPrivate;

class MultiMediaPluginModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        PluginName = Qt::UserRole + 1,
        PluginQmlUrl,
        PluginActive,
        PluginIcoUrl,
    };
    Q_ENUM(Roles)

    MultiMediaPluginModel(const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins = {});
    ~MultiMediaPluginModel();

    void setPlugins(QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> plugins) noexcept;

    qint32 rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setActivePlugin(qint32 activeIndex) noexcept;

    PluginCore::MultimediaPlugin *plugin(qint32 index) const noexcept;

private:
    std::unique_ptr<MultiMediaPluginModelPrivate> d;
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINMODEL_H
