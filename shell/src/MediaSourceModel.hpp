// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <ProviderLoader.hpp>
#include <QAbstractListModel>
#include <memory>

namespace Shell
{
/**
 * The @ref MediaSourceModel is a QAbstractListModel that holds all every MediaSource.
 * It also handels the appearing and disappearing of dynamic MediaSources.
 * For instance a @ref UPnPAV::MediaServer is removed from the network.
 */
class MediaSourceModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * This property holds the active @ref Multimedia::MediaSource.
     * A active @ref Multimedia::MediaSource means that the media of that @ref Multimedia::MediaSource are displayed to
     * the user in the UI.
     */
    Q_PROPERTY(std::shared_ptr<Multimedia::MediaSource> activeMediaSource READ activeMediaSource NOTIFY
                   activeMediaSourceChanged)

public:
    /**
     * The Display Roles for the UI to query the data.
     */
    enum class DisplayRole
    {
        MediaSourceName = Qt::UserRole + 1,
        MediaSourceIocnUrl,
    };

    /**
     * Creates an instance of the ProviderModel
     * @param providerLoader The loader which is used to load the providers.
     */
    MediaSourceModel(std::unique_ptr<Multimedia::ProviderLoader> providerLoader);

    /**
     * Default destructor
     */
    ~MediaSourceModel() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(MediaSourceModel)

    /**
     * Gives the number of provider in the model.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override;

    /**
     * Gives the roles names for the UI to access the for displaying in the model.
     * @return A QHash with all the role names and identifier.
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * Gives the requested data for the UI.
     * The data are requested by the index, index range is from 0..@ref rowCount(QModelIndex) -1
     * Which type of value is requeted is defined by the role parameter.
     * @param index The item index for which the values are requested.
     * @param role The data that shall be returned for item referred by the index.
     * @return A QVariant with the stored data or empty QVariant for an invalid index or role parameter.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const noexcept override;

    /**
     * Activates the @ref Multimedia::MediaSource under the passed indeunder the passed indexxand updates the @ref
     * activeMediaSource property. If an invalid index is passed nothing happens.
     * @param idx The index of the MediaSource that shall be activated.
     */
    Q_INVOKABLE void activateMediaSource(qsizetype idx);

    /**
     * Gives the active @ref Multimedia::MediaSource.
     * This can be a nullptr when currently no @ref Multimedia::MediaSource is active.
     * @return The active @ref Multimedia::MediaSource or nullptr, when no @ref Multimedia::MediaSource is active.
     */
    std::shared_ptr<Multimedia::MediaSource> activeMediaSource();

Q_SIGNALS:
    /**
     * The signal is emitted when the active @ref Multimedia::MediaSource changed.
     */
    void activeMediaSourceChanged();

private Q_SLOTS:
    void onSourceAdded(std::shared_ptr<Multimedia::MediaSource> const &source) noexcept;
    void onSourceRemoved(std::shared_ptr<Multimedia::MediaSource> const &source) noexcept;

private:
    std::unique_ptr<Multimedia::ProviderLoader> mProviderLoader;
    Multimedia::Providers mProviders;
    Multimedia::MediaSources mSources;
    std::shared_ptr<Multimedia::MediaSource> mActiveSource{nullptr};
};

} // namespace Shell
