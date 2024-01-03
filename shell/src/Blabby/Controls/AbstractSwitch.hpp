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
 * Implementation Logic for the Switch
 */
class AbstractSwitch : public AbstractInteractiveControl
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * The @ref Shell::AbstractSwitch::SwitchState of the @ref Shell::AbstractSwitch.
     */
    Q_PROPERTY(
        Shell::AbstractSwitch::SwitchState switchState READ switchState WRITE setSwitchState NOTIFY switchStateChanged)
public:
    /**
     * The state the @ref Shell::AbstractSwitch can have.
     */
    enum class SwitchState
    {
        Active,
        Inactive
    };
    Q_ENUM(SwitchState)

    /*
     * Use base class constructors.
     */
    using AbstractInteractiveControl::AbstractInteractiveControl;

    /**
     * Default destructor
     */
    ~AbstractSwitch() override;

    /*
     * Disabled copy move constructor
     */
    Q_DISABLE_COPY_MOVE(AbstractSwitch)

    /**
     * Gives the @ref Shell::AbstractSwitch::SwitchState of the control
     * @return The @ref Shell::AbstractSwitch::SwitchState of the control.
     */
    SwitchState switchState() const noexcept;

    /**
     * Set the state of the @ref Shell::AbstractSwitch::SwitchState for the @ref Shell::AbstractSwitch.
     * @param state The new state of the control.
     */
    void setSwitchState(SwitchState state) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Shell::AbstractSwitch changes the state.
     */
    void switchStateChanged();

private:
    SwitchState mSwitchState = SwitchState::Inactive;
};

} // namespace Shell
