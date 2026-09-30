// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererModel.hpp"
#include "Queue.hpp"
#include <QAbstractListModel>

namespace Shell
{

/**
 * A thin list model over Blabby's @ref Multimedia::Queue with one row per entry.
 * It follows the Active Renderer of the @ref Shell::MediaRendererModel and passes it to the Queue.
 */
class QueueModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * True while the Queue is Running, false while it's Idle.
     */
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)

public:
    /**
     * The roles of an entry.
     */
    enum class DisplayRole
    {
        Title = Qt::UserRole + 1,
        Artist,
        ArtworkUrl,
        Current,
    };
    Q_ENUM(DisplayRole)

    /**
     * Creates an empty Queue that follows the Active Renderer of the model.
     * @param rendererModel The model whose Active Renderer the Queue plays on, it must outlive the QueueModel.
     */
    explicit QueueModel(MediaRendererModel const& rendererModel);

    /**
     * Default destructor
     */
    ~QueueModel() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(QueueModel)

    /**
     * @copydoc QAbstractItemModel::rowCount(const QModelIndex& parent) const
     */
    int rowCount(QModelIndex const& parent = QModelIndex{}) const noexcept override;

    /**
     * @copydoc QAbstractItemModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * @copydoc QAbstractItemModel::data(const QModelIndex& index, int role) const
     */
    QVariant data(QModelIndex const& index, int role) const noexcept override;

    /**
     * Gives whether the Queue is Running.
     * @return True while the Queue is Running.
     */
    bool isRunning() const noexcept;

    /**
     * Replaces the Queue with the Playables and plays the start entry on the Active Renderer.
     * @see Multimedia::Queue::replace
     * @param playables The Playables in the order they shall be played.
     * @param startIndex The index of the Playable that plays first.
     */
    void replace(Multimedia::Items const& playables, qsizetype startIndex) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the Queue State changed.
     */
    void runningChanged();

private:
    void onCurrentEntryChanged() noexcept;
    void notifyCurrentChanged(std::optional<qsizetype> row) noexcept;

    MediaRendererModel const& mRendererModel;
    Multimedia::Queue mQueue;
    std::optional<qsizetype> mCurrentRow;
};

} // namespace Shell
