// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "blabbyupnpav_export.h"
#include <QString>
#include <optional>

namespace UPnPAV
{

enum class BLABBYUPNPAV_EXPORT ConnectionInfoDirection
{
    Output,
    Input
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
    quint32 rcsId;
    quint32 avTransportId;
    QString protoclInfo;
    QString peerConnectionManager;
    quint32 peerConnectionId;
    ConnectionInfoDirection direction;
    ConnectionInfoStatus status;

    BLABBYUPNPAV_EXPORT friend bool operator==(ConnectionInfo const &lhs, ConnectionInfo const &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(ConnectionInfo const &lhs, ConnectionInfo const &rhs);
};

/**
 * Converts the XML response from a GetConnectionInfo call to connection info
 */
class BLABBYUPNPAV_EXPORT CurrentConnectionInfoResult final
{
public:
    CurrentConnectionInfoResult(const QString &xmlResponse);

    const ConnectionInfo &connectionInfo() const noexcept;

private:
    static std::optional<quint32> convertU32Value(QString const &rawValue) noexcept;
    static std::optional<ConnectionInfoDirection> convertDirection(QString const &rawValue) noexcept;
    static std::optional<ConnectionInfoStatus> convertStatus(QString const &rawValue) noexcept;

private:
    ConnectionInfo mConnectionInfo;
};

} // namespace UPnPAV
