// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapCall.hpp"

#include <utility>

namespace UPnPAV
{

SoapCall::SoapCall() = default;
SoapCall::SoapCall(ServiceControlPointDefinition scpd, SCPDAction action)
    : mScpd{std::move(scpd)}
    , mAction(std::move(action))
{
}

SoapCall::~SoapCall() = default;

SCPDAction const& SoapCall::action() const noexcept
{
    return mAction;
}

ServiceControlPointDefinition const& SoapCall::scpd() const noexcept
{
    return mScpd;
}

} // namespace UPnPAV
