// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QQuickItem>

namespace Shell
{

/**
 * Implementation logic for the AbstractListEntry
 */
class AbstractListEntry : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * The Title of the ListEntry
     */
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    /**
     * The optional icon of the ListEntry
     */
    Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
public:
    /**
     * Inherited every QQuickItem constructor
     */
    using QQuickItem::QQuickItem;

    /**
     * Default destructor
     */
    ~AbstractListEntry() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(AbstractListEntry)

    /**
     * Gives the title for the ListEntry.
     * @return The title of the ListEnty.
     */
    [[nodiscard]] QString title() const noexcept;

    /**
     * Sets the title for the ListEntry.
     * Emits @titleChanged when title is changed.
     * @param title The new title of the entry.
     */
    void setTitle(QString const &title) noexcept;

    /**
     * Gives the URL for the icon of the ListEntry.
     * @return The URL for the icon for the ListEntry.
     */
    [[nodiscard]] QString iconUrl() const noexcept;

    /**
     * Sets the iconUrl for the ListEntry.
     * Emits @iconUrlChanged when the iconUrlChanged is changed.
     * @param iconUrl The new URL for the icon of the ListEntry.
     */
    void setIconUrl(QString const &iconUrl) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when ever a list entry is clicked.
     */
    void clicked();

    /**
     * This signal is emitted when the title of the ListEntry is changed.
     */
    void titleChanged();

    /**
     * This signal is emitted when the iconUrl of the ListEntry is changed.
     */
    void iconUrlChanged();

private:
    QString mTitle;
    QString mIconUrl;
};

} // namespace Shell
