// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "blabbyupnpav_export.h"
#include <QString>
#include <QVector>
#include <optional>

namespace UPnPAV
{

/**
 * The Protocol describes one specific protocol supported by the device. An asterix(*) is a wildcared and matches with
 * everything during string comparsions to determine the supported protocol in the control point. struct Protocol
 */
struct BLABBYUPNPAV_EXPORT Protocol
{
    QString protocol;
    QString network;
    QString contentFormat;
    QString additionalInfo;

    BLABBYUPNPAV_EXPORT friend bool operator==(const Protocol &lhs, const Protocol &rhs) noexcept;
    BLABBYUPNPAV_EXPORT friend bool operator!=(const Protocol &lhs, const Protocol &rhs) noexcept;
};

/**
 * This struct is the result of ConnectionManager::GetProtocolInfo call on a UPnPAV media device. The result contains
 * the supported source and sink protocols of the device.
 */
class BLABBYUPNPAV_EXPORT ProtocolInfoResult
{
public:
    ProtocolInfoResult(const QString &xmlResponse);

    const QVector<Protocol> &getSourceProtocols() const noexcept;
    const QVector<Protocol> &getSinkProtocols() const noexcept;

private:
    static std::optional<QVector<Protocol>> parseProtocolResponse(const QString &rawResult) noexcept;

private:
    QVector<Protocol> mSourceProtocols;
    QVector<Protocol> mSinkProtocols;
};

} // namespace UPnPAV
