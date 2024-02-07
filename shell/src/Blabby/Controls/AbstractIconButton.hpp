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

    /**
     * The style of the @ref Shell::AbstractIconButton
     * Default: Default
     */
    Q_PROPERTY(Shell::AbstractIconButton::Style style READ style WRITE setStyle NOTIFY styleChanged)

public:
    /**
     * Different Styles of the @ref Shell::AbstractIconButton
     */
    enum class Style
    {
        /**
         * The default style.
         */
        Default,

        /**
         * In this style the @ref Shell::AbstractIconButton will have a background per default.
         */
        Tonal,
    };
    Q_ENUM(Style)

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
    void setSource(QUrl const& source) noexcept;

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

    /**
     * Gives the style of the @ref Shell::AbstractIconButton.
     * @return The style of the button
     */
    AbstractIconButton::Style style() const noexcept;

    /**
     * Sets the new style of the button
     * @param style The new style of the @ref Shell::AbstractIconButton
     */
    void setStyle(AbstractIconButton::Style style) noexcept;

Q_SIGNALS:
    /**
     * The signal is emitted source for the icon of the button is changed.
     */
    void sourceChanged();

    /**
     * This signal is emitted when the border property is changed.
     */
    void borderChanged();

    /**
     * This singal is emitted when the style of the IconButton is changed.
     */
    void styleChanged();

private:
    QUrl mIconSource;
    bool mBorder{false};
    Style mStyle = Style::Default;
};
} // namespace Shell
