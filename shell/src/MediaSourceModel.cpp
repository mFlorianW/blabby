// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaSourceModel.hpp"
#include "LoggingCategories.hpp"
#include "ProviderDirs.hpp"

namespace Shell
{

MediaSourceModel::MediaSourceModel(std::unique_ptr<Multimedia::ProviderLoader> providerLoader)
    : QAbstractListModel{}
    , mProviderLoader{std::move(providerLoader)}
{
    Q_ASSERT(mProviderLoader != nullptr);
    mProviders = mProviderLoader->load({QStringLiteral(DEFAULT_PROVIDER_DIR)});
    beginResetModel();
    for (auto const &provider : std::as_const(mProviders))
    {
        connect(provider.get(), &Multimedia::Provider::sourceAdded, this, &MediaSourceModel::onSourceAdded);
        connect(provider.get(), &Multimedia::Provider::sourceRemoved, this, &MediaSourceModel::onSourceRemoved);
        if (provider->init())
        {
            mSources.append(provider->sources());
        }
        else
        {
            disconnect(provider.get());
        }
    }
    endResetModel();
}

MediaSourceModel::~MediaSourceModel() = default;

int MediaSourceModel::rowCount(const QModelIndex &parent) const noexcept
{
    Q_UNUSED(parent)
    return static_cast<int>(mSources.size());
}

QHash<int, QByteArray> MediaSourceModel::roleNames() const noexcept
{
    static auto roleNames = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceName), QByteArray{"mediaSourceName"}),
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceIocnUrl),
                       QByteArray{"mediaSourceIconUrl"})};
    return roleNames;
}

QVariant MediaSourceModel::data(const QModelIndex &index, int role) const noexcept
{
    if (not index.isValid() or (index.row() >= mSources.size()))
    {
        qCritical(shell) << "Invalided index for requesting data. Index:" << index << "Source size:" << mSources.size();
        return {};
    }

    const auto item = mSources.at(index.row());
    const auto dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::MediaSourceName)
    {
        return item->sourceName();
    }
    else if (dispRole == DisplayRole::MediaSourceIocnUrl)
    {
        return item->iconUrl();
    }

    return {};
}

void MediaSourceModel::onSourceAdded(std::shared_ptr<Multimedia::MediaSource> const &source) noexcept
{
    const auto newIndex = static_cast<int>(mSources.size());
    beginInsertRows(index(newIndex), newIndex, newIndex);
    mSources.append(source);
    endInsertRows();
}

void MediaSourceModel::onSourceRemoved(std::shared_ptr<Multimedia::MediaSource> const &source) noexcept
{
    auto sourceIndex = std::find_if(mSources.cbegin(),
                                    mSources.cend(),
                                    [&](std::shared_ptr<Multimedia::MediaSource> const &src) { return src == source; });
    if (sourceIndex != mSources.cend())
    {
        const auto idx = static_cast<int>(std::distance(mSources.cbegin(), sourceIndex));
        beginRemoveRows(index(idx), idx, idx);
        mSources.remove(static_cast<int>(idx));
        endRemoveRows();
    }
}

} // namespace Shell
