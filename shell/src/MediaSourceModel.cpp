// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
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
    for (auto const& provider : std::as_const(mProviders)) {
        connect(provider.get(), &Multimedia::Provider::sourceAdded, this, &MediaSourceModel::onSourceAdded);
        connect(provider.get(), &Multimedia::Provider::sourceRemoved, this, &MediaSourceModel::onSourceRemoved);
        if (provider->init()) {
            mSources.append(provider->sources());
        } else {
            disconnect(provider.get());
        }
    }
    endResetModel();
}

MediaSourceModel::~MediaSourceModel() = default;

int MediaSourceModel::rowCount(QModelIndex const& parent) const noexcept
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

QVariant MediaSourceModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or (index.row() >= mSources.size())) {
        qCritical(shell) << "Failed to request data. Error: invalid index:" << index.row()
                         << "Source size:" << mSources.size();
        return {};
    }

    auto const item = mSources.at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::MediaSourceName) {
        return item->sourceName();
    } else if (dispRole == DisplayRole::MediaSourceIocnUrl) {
        return item->iconUrl();
    }

    return {};
}

void MediaSourceModel::activateMediaSource(qsizetype idx)
{
    if (idx >= mSources.size()) {
        qCCritical(shell) << "Failed to activate MediaSource. Error: invalid index passed";
        return;
    }

    mActiveSource = mSources.at(idx);
    Q_EMIT activeMediaSourceChanged();
}

std::shared_ptr<Multimedia::Source> MediaSourceModel::activeMediaSource()
{
    return mActiveSource;
}

void MediaSourceModel::onSourceAdded(std::shared_ptr<Multimedia::Source> const& source) noexcept
{
    auto const newIndex = static_cast<int>(mSources.size());
    beginInsertRows(index(newIndex), newIndex, newIndex);
    mSources.append(source);
    endInsertRows();
}

void MediaSourceModel::onSourceRemoved(std::shared_ptr<Multimedia::Source> const& source) noexcept
{
    auto sourceIndex =
        std::find_if(mSources.cbegin(), mSources.cend(), [&](std::shared_ptr<Multimedia::Source> const& src) {
            return src == source;
        });
    if (sourceIndex != mSources.cend()) {
        auto const idx = static_cast<int>(std::distance(mSources.cbegin(), sourceIndex));
        beginRemoveRows(index(idx), idx, idx);
        mSources.remove(static_cast<int>(idx));
        qCDebug(shell) << "Remove MediaSource under index:" << idx << "from sources. Address:" << source.get()
                       << "iter:" << sourceIndex->get();
        endRemoveRows();
    }
}

} // namespace Shell
