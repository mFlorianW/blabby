// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "InvalidDeviceDescription.h"

namespace UPnPAV
{

InvalidDeviceDescription::InvalidDeviceDescription(const QString &errorMessage)
    : m_errorMessage(errorMessage.toLocal8Bit())
{
}

const char *InvalidDeviceDescription::what() const noexcept
{
    return m_errorMessage;
}

void InvalidDeviceDescription::raise() const
{
    throw *this;
}

InvalidDeviceDescription *InvalidDeviceDescription::clone() const
{
    return new InvalidDeviceDescription(*this);
}

} // namespace UPnPAV
