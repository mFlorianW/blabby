// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

/**
 * The Protocol describes one specific protocol supported by the device. An asterix(*) is a wildcared and matches with
 * everything during string comparsions to determine the supported protocol in the control point
 */
struct BLABBYUPNPAV_EXPORT Protocol
{
    QString protocol;
    QString network;
    QString contentFormat;
    QString additionalInfo;

    BLABBYUPNPAV_EXPORT friend bool operator==(const Protocol &lhs, const Protocol &rhs) noexcept;
    BLABBYUPNPAV_EXPORT friend bool operator!=(const Protocol &lhs, const Protocol &rhs) noexcept;

    /**
     * Creates a @ref UPnPAV::Protocol from a string.
     * The string must have the format:
     *      <protocol>:<network>:<contentFormat>:<additionalInfo>
     * @return Valid optional if parsing runs successful othwise a std::nullopt
     */
    static std::optional<Protocol> create(QString const &rawProtocol);
};

} // namespace UPnPAV

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug d, const UPnPAV::Protocol &proto);
