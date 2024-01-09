// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QQuickItem>

namespace Shell
{

/**
 * Implentation Logic for the Dialog Contol
 */
class AbstractDialog : public QQuickItem
{
    Q_OBJECT

    /**
     * This constant contains the height of the header of the dialog.
     */
    Q_PROPERTY(qreal headerHeight READ headerHeight CONSTANT)

    /**
     * The title of the dialg
     */
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    /**
     * The content item of the dialog
     * If the contentItem does not a height or width the item is automatically expanded to size of the dialog.
     */
    Q_PROPERTY(QQuickItem* contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged)

    QML_ELEMENT
public:
    /*
     * Inherit base class constructors.
     */
    using QQuickItem::QQuickItem;

    /**
     * Default destructor
     */
    ~AbstractDialog() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(AbstractDialog)

    /**
     * Gives the height of the header.
     * @return The height of the header.
     */
    qreal headerHeight() const noexcept;

    /**
     * Gives the title of the Shell::AbstractDialog.
     * @return The title of the @ref Shell::AbstractDialog.
     */
    QString const& title() const noexcept;

    /**
     * Sets a new title for the @ref Shell::AbstractDialog.
     * @param title The new title.
     */
    void setTitle(QString const& title) noexcept;

    /**
     * Gives the content item of the @ref Shell::AbstractDialog.
     * @return The content item of the @ref Shell::AbstractDialog.
     */
    QQuickItem* contentItem() const noexcept;

    /**
     * Sets the new contentItem of the @ref Shell::AbstractDialog
     * If the contentItem does not a height or width the item is automatically expanded to size of the dialog.
     * @param contentItem The new content item for the dialog.
     */
    void setContentItem(QQuickItem* contentItem) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the title of the @ref Shell::AbstractDialog is changed.
     */
    void titleChanged();

    /**
     * This signal is emitted when the contentItem of the @ref Shell::AbstractDialog is changed.
     */
    void contentItemChanged();

private:
    QString mTitle;
    QQuickItem* mContentItem;
    qreal mHeaderHeight = 52;
};

} // namespace Shell
