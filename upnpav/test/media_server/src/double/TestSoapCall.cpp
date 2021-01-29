// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestSoapCall.h"

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

} //namespace UPnPAV
