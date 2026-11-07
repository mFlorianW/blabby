// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <ProviderLoader.hpp>
#include <QAbstractListModel>
#include <QTimer>
#include <chrono>
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
    Q_PROPERTY(
        std::shared_ptr<Multimedia::Source> activeMediaSource READ activeMediaSource NOTIFY activeMediaSourceChanged)

    /**
     * This property is true for the scan duration after a Rescan started by @ref MediaSourceModel::rescan.
     * The Providers don't report the end of a Rescan, the scan duration is the time a MediaServer on the network has
     * to answer it.
     */
    Q_PROPERTY(bool scanning READ isScanning NOTIFY scanningChanged)

public:
    /**
     * The Display Roles for the UI to query the data.
     */
    enum class DisplayRole
    {
        MediaSourceName = Qt::UserRole + 1,
        MediaSourceIocnUrl,
        /**
         * True when the @ref Multimedia::Source is the Active Source.
         */
        Active,
    };

    /**
     * The time @ref MediaSourceModel::scanning stays true after a Rescan, the time the search on the network takes.
     */
    static constexpr auto DefaultScanDuration = std::chrono::milliseconds{3000};

    /**
     * Creates an instance of the MediaSourceModel that holds the Sources of the passed Providers.
     * @param providers The loaded Providers, they are already initialized.
     * @param scanDuration The time @ref MediaSourceModel::scanning stays true after a Rescan.
     */
    explicit MediaSourceModel(Multimedia::Providers providers,
                              std::chrono::milliseconds scanDuration = DefaultScanDuration);

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
    int rowCount(QModelIndex const& parent = QModelIndex()) const noexcept override;

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
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const noexcept override;

    /**
     * Makes the @ref Multimedia::Source under the passed index the Active Source, updates the @ref activeMediaSource
     * property and notifies a change of the @ref DisplayRole::Active role for the previous and the new Active Source.
     * If an invalid index or the index of the Active Source is passed nothing happens.
     * @param idx The index of the MediaSource that shall be activated.
     */
    Q_INVOKABLE void activateMediaSource(qsizetype idx);

    /**
     * Gives the active @ref Multimedia::MediaSource.
     * This can be a nullptr when currently no @ref Multimedia::MediaSource is active.
     * @return The active @ref Multimedia::MediaSource or nullptr, when no @ref Multimedia::MediaSource is active.
     */
    std::shared_ptr<Multimedia::Source> activeMediaSource();

    /**
     * Gives true for the scan duration after a Rescan.
     * @return True while a Rescan is running.
     */
    bool isScanning() const noexcept;

    /**
     * Rescans every Provider by triggering its @ref Multimedia::rescanActionKey action, the Sources it finds or misses
     * are reported as added or removed. Nothing happens while a Rescan is running.
     */
    Q_INVOKABLE void rescan();

Q_SIGNALS:
    /**
     * The signal is emitted when the active @ref Multimedia::MediaSource changed.
     */
    void activeMediaSourceChanged();

    /**
     * The signal is emitted when the Active Source disappeared and is no longer the Active Source.
     * @param sourceName The name of the Source that disappeared.
     */
    void activeMediaSourceDisappeared(QString const& sourceName);

    /**
     * The signal is emitted when the @ref MediaSourceModel::scanning property changed.
     */
    void scanningChanged();

private Q_SLOTS:
    void onSourceAdded(std::shared_ptr<Multimedia::Source> const& source) noexcept;
    void onSourceRemoved(std::shared_ptr<Multimedia::Source> const& source) noexcept;

private:
    Multimedia::Providers mProviders;
    Multimedia::MediaSources mSources;
    std::shared_ptr<Multimedia::Source> mActiveSource{nullptr};
    QTimer mScanTimer;
};

} // namespace Shell
