// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Protocol.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QString>
#include <QVector>
#include <optional>

namespace UPnPAV
{

/**
 * This is the result of ConnectionManager::GetProtocolInfo call on a UPnPAV media device. The result contains
 * the supported source and sink protocols of the device.
 */
class BLABBYUPNPAV_EXPORT GetProtocolInfoResponse
{
public:
    GetProtocolInfoResponse(QString const &xmlResponse);
    GetProtocolInfoResponse(QString const &xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);

    QVector<Protocol> const &sourceProtocols() const noexcept;
    QVector<Protocol> const &sinkProtocols() const noexcept;

private:
    static std::optional<QVector<Protocol>> parseProtocolResponse(QString const &rawResult) noexcept;

private:
    QVector<Protocol> mSourceProtocols;
    QVector<Protocol> mSinkProtocols;
};

} // namespace UPnPAV
