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
#include "MultiMediaPluginModel.h"
#include "MultimediaPlugin.h"
#include "MultiMediaPluginModel_p.h"

namespace Shell
{

MultiMediaPluginModelPrivate::MultiMediaPluginModelPrivate(const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins)
    : mPlugins{ plugins }
{
}

MultiMediaPluginModel::MultiMediaPluginModel(const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins)
    : QAbstractListModel{}
    , d{ std::make_unique<MultiMediaPluginModelPrivate>(plugins) }
{
}

void MultiMediaPluginModel::setPlugins(QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> plugins) noexcept
{
    beginResetModel();
    d->mPlugins = plugins;
    endResetModel();
}

MultiMediaPluginModel::~MultiMediaPluginModel() = default;

qint32 MultiMediaPluginModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->mPlugins.size();
}

QVariant MultiMediaPluginModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || (index.row() < 0) || (index.row() >= d->mPlugins.size()))
    {
        return {};
    }

    if(role == Roles::PluginName)
    {
        return QVariant{ d->mPlugins.at(index.row())->pluginName() };
    }
    else if(role == Roles::PluginQmlUrl)
    {
        return QVariant{ d->mPlugins.at(index.row())->mainQMLUrl() };
    }
    else if(role == Roles::PluginActive)
    {
        return QVariant{ d->mActiveIndex == index.row() };
    }
    else if(role == Roles::PluginIcoUrl)
    {
        return QVariant{ d->mPlugins.at(index.row())->pluginIconUrl() };
    }

    return {};
}

QHash<int, QByteArray> MultiMediaPluginModel::roleNames() const
{
    return { { std::make_pair(MultiMediaPluginModel::PluginName, "pluginName"),
               std::make_pair(MultiMediaPluginModel::PluginQmlUrl, "qmlUrl"),
               std::make_pair(MultiMediaPluginModel::PluginActive, "pluginActive"),
               std::make_pair(MultiMediaPluginModel::PluginIcoUrl, "pluginIconUrl") } };
}

void MultiMediaPluginModel::setActivePlugin(qint32 activeIndex) noexcept
{
    if(d->mActiveIndex == activeIndex)
    {
        return;
    }

    d->mActiveIndex = activeIndex;
    Q_EMIT dataChanged(index(activeIndex), index(activeIndex), { PluginActive });
}

} // namespace Shell
