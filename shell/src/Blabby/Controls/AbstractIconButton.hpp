// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QQuickItem>

namespace Shell
{
/**
 * Base implementation for an IconButton.
 * The class has all the logic for the button without any visual aspects.
 */
class AbstractIconButton : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    QML_ELEMENT
public:
    /**
     * Inherited Constructors
     */
    using QQuickItem::QQuickItem;

    /**
     * Destructor
     */
    ~AbstractIconButton() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(AbstractIconButton)

    /**
     * Gives the icon source for the button.
     * @return The icon source for the button.
     */
    QUrl source() const noexcept;

    /**
     * Sets the icon source for the button.
     * @param source The new source for the button.
     */
    void setSource(QUrl const &source) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @AbstractIconButton is clicked
     */
    void clicked();

    /**
     * The signal is emitted source for the icon of the button is changed.
     */
    void sourceChanged();

private:
    QUrl mIconSource;
};
} // namespace Shell
