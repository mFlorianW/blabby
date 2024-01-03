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
 * Base class for every interactive UI control.
 * An @ref Shell::AbstractInteractiveControl has different @ref Shell::AbstractInteractiveControl::ControlState.
 * The @ref Shell::AbstractInteractiveControl::ControlState can be used to display states of the control.
 */
class AbstractInteractiveControl : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * This property is true when the control is hovered, otherwise false
     */
    Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged)

    /**
     * This property is true when the control is pressed, otherwise false
     */
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)

public:
    /**
     * Default constructor
     */
    AbstractInteractiveControl();

    /**
     * Default destructor
     */
    ~AbstractInteractiveControl() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(AbstractInteractiveControl)

    /**
     * This property gives the hover state.
     * @return True @ref Shell::AbstractInteractiveControl is hovered otherwise false.
     */
    bool isHovered() const noexcept;

    /**
     * This property gives the pressed state.
     * @return True @ref Shell::AbstractInteractiveControl is hovered otherwise false.
     */
    bool isPressed() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when ever the item is clicked.
     */
    void clicked();

    /**
     * This signal is emitted when the hover state is changed.
     */
    void hoveredChanged();

    /**
     * This signal is emitted when ever the @ref Shell::AbstractInteractiveControl is pressed.
     */
    void pressedChanged();

    /**
     * This signal is emitted when the @ref Shell::AbstractInteractiveControl @ref is changed.
     */
    void controlStateChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

private:
    bool mHoverState = false;
    bool mPressState = false;
};

} // namespace Shell
