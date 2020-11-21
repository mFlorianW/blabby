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
#ifndef MULTIMEDIAPLUGINMODEL_H
#define MULTIMEDIAPLUGINMODEL_H

#include <QAbstractListModel>
#include <memory>
#include <qqml.h>

namespace PluginCore
{
class MultiMediaPlugin;
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

    MultiMediaPluginModel(const QVector<const PluginCore::MultiMediaPlugin *> &plugins = {});
    ~MultiMediaPluginModel();

    void setPlugins(QVector<const PluginCore::MultiMediaPlugin *> plugins) noexcept;

    qint32 rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setActivePlugin(qint32 activeIndex) noexcept;

private:
    std::unique_ptr<MultiMediaPluginModelPrivate> d;
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINMODEL_H
