// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QObject>
#include <QString>
#include <optional>

namespace UPnPAV
{

enum class BLABBYUPNPAV_EXPORT ConnectionInfoDirection
{
    Output,
    Input,
    Unknown
};

enum class BLABBYUPNPAV_EXPORT ConnectionInfoStatus
{
    OK,
    ContentFormatMismatch,
    InsufficientBandwidth,
    UnreliableChannel,
    Unknown
};

struct BLABBYUPNPAV_EXPORT ConnectionInfo
{
    quint32 rcsId{0};
    quint32 avTransportId{0};
    QString protoclInfo{""};
    QString peerConnectionManager{""};
    quint32 peerConnectionId{0};
    ConnectionInfoDirection direction{ConnectionInfoDirection::Unknown};
    ConnectionInfoStatus status{ConnectionInfoStatus::Unknown};

    BLABBYUPNPAV_EXPORT friend bool operator==(ConnectionInfo const &lhs, ConnectionInfo const &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(ConnectionInfo const &lhs, ConnectionInfo const &rhs);
};

/**
 * Converts the XML response from a GetConnectionInfo call to connection info
 */
class BLABBYUPNPAV_EXPORT GetCurrentConnectionInfoResponse final
{
public:
    GetCurrentConnectionInfoResponse(QString const &xmlResponse,
                                     ServiceControlPointDefinition const &scpd,
                                     SCPDAction action);
    Q_DISABLE_COPY_MOVE(GetCurrentConnectionInfoResponse)
    ~GetCurrentConnectionInfoResponse();

    ConnectionInfo const &connectionInfo() const noexcept;

private:
    static std::optional<quint32> convertU32Value(QString const &rawValue) noexcept;
    static std::optional<ConnectionInfoDirection> convertDirection(QString const &rawValue) noexcept;
    static std::optional<ConnectionInfoStatus> convertStatus(QString const &rawValue) noexcept;

private:
    ConnectionInfo mConnectionInfo;
};

} // namespace UPnPAV
