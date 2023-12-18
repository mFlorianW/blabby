// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Protocol.hpp"

namespace UPnPAV
{

bool operator==(const Protocol &lhs, const Protocol &rhs) noexcept
{
    // clang-format off
    return (lhs.network == rhs.network or (lhs.network == "*" or rhs.network == "*")) and
           (lhs.protocol == rhs.protocol or (lhs.protocol == "*" or rhs.protocol == "*")) and
           (lhs.contentFormat == rhs.contentFormat or (lhs.contentFormat == "*" or rhs.contentFormat == "*")) and
           (lhs.additionalInfo == rhs.additionalInfo or (lhs.additionalInfo== "*" or rhs.additionalInfo == "*"));
    // clang-format on
}

bool operator!=(const Protocol &lhs, const Protocol &rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
