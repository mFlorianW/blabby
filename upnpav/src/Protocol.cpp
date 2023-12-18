// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Protocol.hpp"
#include <QDebug>
#include <QStringList>

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

std::optional<Protocol> Protocol::create(QString const &rawProtocol)
{
    const auto parts = rawProtocol.split(":");
    if (parts.size() < 3) {
        return std::nullopt;
    }

    if (parts.size() == 3) {
        return Protocol{.protocol = parts.at(0),
                        .network = parts.at(1),
                        .contentFormat = parts.at(2),
                        .additionalInfo = QString("")};
    }

    if (parts.size() == 4) {
        return Protocol{.protocol = parts.at(0),
                        .network = parts.at(1),
                        .contentFormat = parts.at(2),
                        .additionalInfo = parts.at(3)};
    }
    return std::nullopt;
}

} // namespace UPnPAV

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug d, const UPnPAV::Protocol &proto)
{
    QDebugStateSaver saver(d);

    d.nospace().noquote() << "Protocol:" << proto.protocol << "\n";
    d.nospace().noquote() << "Network:" << proto.network << "\n";
    d.nospace().noquote() << "Content Format:" << proto.contentFormat << "\n";
    d.nospace().noquote() << "Additional Info:" << proto.additionalInfo << "\n";

    return d;
}
