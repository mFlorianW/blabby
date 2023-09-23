// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MultiMediaPluginModel.h"
#include "MultiMediaPluginModel_p.h"
#include "MultimediaPlugin.h"

namespace Shell
{

MultiMediaPluginModelPrivate::MultiMediaPluginModelPrivate(
    const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins)
    : mPlugins{plugins}
{
}

MultiMediaPluginModel::MultiMediaPluginModel(const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins)
    : QAbstractListModel{}
    , d{std::make_unique<MultiMediaPluginModelPrivate>(plugins)}
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
    if (!index.isValid() || (index.row() < 0) || (index.row() >= d->mPlugins.size()))
    {
        return {};
    }

    if (role == Roles::PluginName)
    {
        return QVariant{d->mPlugins.at(index.row())->pluginName()};
    }
    else if (role == Roles::PluginActive)
    {
        return QVariant{d->mActiveIndex == index.row()};
    }
    else if (role == Roles::PluginIcoUrl)
    {
        return QVariant{d->mPlugins.at(index.row())->pluginIconUrl()};
    }

    return {};
}

QHash<int, QByteArray> MultiMediaPluginModel::roleNames() const
{
    return {{std::make_pair(MultiMediaPluginModel::PluginName, "pluginName"),
             std::make_pair(MultiMediaPluginModel::PluginQmlUrl, "qmlUrl"),
             std::make_pair(MultiMediaPluginModel::PluginActive, "pluginActive"),
             std::make_pair(MultiMediaPluginModel::PluginIcoUrl, "pluginIconUrl")}};
}

void MultiMediaPluginModel::setActivePlugin(qint32 activeIndex) noexcept
{
    if (d->mActiveIndex == activeIndex)
    {
        return;
    }

    d->mActiveIndex = activeIndex;
    Q_EMIT dataChanged(index(activeIndex), index(activeIndex), {PluginActive});
}

PluginCore::MultimediaPlugin *MultiMediaPluginModel::plugin(qint32 index) const noexcept
{
    if (index < 0 || index > d->mPlugins.size())
    {
        return nullptr;
    }

    return d->mPlugins.at(index).get();
}

} // namespace Shell
