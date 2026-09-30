// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestSoapCall.hpp"

namespace UPnPAV
{

TestSoapCall::TestSoapCall() = default;

TestSoapCall::TestSoapCall(ServiceControlPointDefinition scpd, SCPDAction action)
    : SoapCall{std::move(scpd), std::move(action)}
{
}

bool TestSoapCall::hasFinishedSuccesful() const noexcept
{
    return true;
}

bool TestSoapCall::isDeviceUnreachable() const noexcept
{
    return false;
}

QString TestSoapCall::rawMessage() const noexcept
{
    return "";
}

} // namespace UPnPAV
