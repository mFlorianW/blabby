// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "AbstractInteractiveControl.hpp"

namespace Shell
{

/**
 * Implementation logic for the AbstractListEntry
 */
class AbstractListEntry : public AbstractInteractiveControl
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

    /**
     * The optional trailing element for the entry
     */
    Q_PROPERTY(QQuickItem* trailingElement READ trailingElement WRITE setTrailingElement NOTIFY trailingElementChanged)
public:
    /**
     * Inherited every QQuickItem constructor
     */
    using AbstractInteractiveControl::AbstractInteractiveControl;

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
    void setTitle(QString const& title) noexcept;

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
    void setIconUrl(QString const& iconUrl) noexcept;

    /**
     * Gives the trailingElement for the @ref Shell::AbstractListEntry.
     * The trailingElement is positioned on the right side of the entry.
     * @return The trailing element or a nullptr when none is set.
     */
    QQuickItem* trailingElement() const noexcept;

    /**
     * Sets the trailing for the @ref Shell::AbstractListEntry.
     * @param trailingElement The new trailing element for the @ref Shell::AbstractListEntry.
     */
    void setTrailingElement(QQuickItem* trailingElement) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the title of the @ref Shell::AbstractListEntry is changed.
     */
    void titleChanged();

    /**
     * This signal is emitted when the iconUrl of the @ref Shell::AbstractListEntry is changed.
     */
    void iconUrlChanged();

    /**
     * This signal is emitted when the trailingElement of the @ref Shell::AbstractListEntry is changed.
     */
    void trailingElementChanged();

private:
    QString mTitle;
    QString mIconUrl;
    QQuickItem* mTrailingElement = nullptr;
};

} // namespace Shell
