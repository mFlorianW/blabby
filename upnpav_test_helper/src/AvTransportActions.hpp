// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include <SCPDAction.hpp>

namespace UPnPAV
{
SCPDAction createSetAVTransportURIAction() noexcept;

SCPDAction createSetNextAVTransportURIAction() noexcept;

SCPDAction createGetMediaInfoAction() noexcept;

SCPDAction createGetTransportInfoAction() noexcept;

SCPDAction createGetPositionInfoAction() noexcept;

SCPDAction createGetDeviceCapabilitiesAction() noexcept;

SCPDAction createGetTransportSettingsAction() noexcept;

SCPDAction createStopAction() noexcept;

SCPDAction createPlayAction() noexcept;

SCPDAction createSeekAction() noexcept;

SCPDAction createNextAction() noexcept;

SCPDAction createPreviousAction() noexcept;
} // namespace UPnPAV
