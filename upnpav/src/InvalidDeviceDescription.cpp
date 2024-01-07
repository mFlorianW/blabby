// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "InvalidDeviceDescription.hpp"

namespace UPnPAV
{

InvalidDeviceDescription::InvalidDeviceDescription(QString const &errorMessage)
    : m_errorMessage(errorMessage.toLocal8Bit())
{
}

char const *InvalidDeviceDescription::what() const noexcept
{
    return m_errorMessage;
}

void InvalidDeviceDescription::raise() const
{
    throw *this;
}

InvalidDeviceDescription *InvalidDeviceDescription::clone() const
{
    return new InvalidDeviceDescription(*this); // NOLINT cppcoreguidelines-owning-memory
}

} // namespace UPnPAV
