// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "AbstractInteractiveControl.hpp"

namespace Shell
{
/**
 * Base implementation for an IconButton.
 * The class has all the logic for the button without any visual aspects.
 */
class AbstractIconButton : public AbstractInteractiveControl
{
    Q_OBJECT
    QML_ELEMENT
    /**
     * The icon source URL for the IconButton
     */
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

    /**
     * Enables the Border for the IconButton
     * Default: False
     */
    Q_PROPERTY(bool border READ border WRITE setBorder NOTIFY borderChanged)

public:
    /**
     * Inherited Constructors
     */
    using AbstractInteractiveControl::AbstractInteractiveControl;

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

    /**
     * Display a border around the @ref Shell::AbstractIconButton.
     * Default: False
     * @return True around the border a border is displayed. False no border is displayed.
     */
    bool border() const noexcept;

    /**
     * Enable or disable the border state for the @ref Shell::AbstractIconButton
     * border The new state of the border. True enables the border, false disables the border.
     */
    void setBorder(bool border) noexcept;

Q_SIGNALS:
    /**
     * The signal is emitted source for the icon of the button is changed.
     */
    void sourceChanged();

    /**
     * This signal is emitted when the border property is changed.
     */
    void borderChanged();

private:
    QUrl mIconSource;
    bool mBorder{false};
};
} // namespace Shell
