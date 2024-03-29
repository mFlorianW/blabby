// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestSoapCall.hpp"

namespace UPnPAV
{

bool TestSoapCall::hasFinishedSuccesful() const noexcept
{
    return true;
}

QString TestSoapCall::rawMessage() const noexcept
{
    return "";
}

} // namespace UPnPAV
