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
#include "MultiMediaPlugin.h"
#include "MultiMediaPluginModel_p.h"

namespace Shell
{

MultiMediaPluginModel::MultiMediaPluginModel(const QVector<PluginCore::MultiMediaPlugin *> &plugins)
    : QAbstractListModel{}
    , d{ std::make_unique<MultiMediaPluginModelPrivate>(plugins) }
{
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
        return QVariant{ d->mPlugins.at(index.row())->getPluginName() };
    }

    return {};
}

} // namespace Shell
