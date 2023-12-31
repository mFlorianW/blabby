// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "AbstractSwitch.hpp"

namespace Shell
{

AbstractSwitch::~AbstractSwitch() = default;

AbstractSwitch::SwitchState AbstractSwitch::switchState() const noexcept
{
    return mSwitchState;
}

void AbstractSwitch::setSwitchState(SwitchState state) noexcept
{
    if (mSwitchState != state) {
        mSwitchState = state;
        Q_EMIT switchStateChanged();
    }
}

} // namespace Shell
