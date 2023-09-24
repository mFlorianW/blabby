// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "CurrentConnectionInfoResult.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <optional>
#include <qstringliteral.h>

namespace UPnPAV
{

CurrentConnectionInfoResult::CurrentConnectionInfoResult(QString const &xmlResponse)
{
    auto responseReader = QXmlStreamReader{xmlResponse};
    while (responseReader.readNext() && !responseReader.atEnd() && !responseReader.hasError())
    {
        if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("RcsID"))
        {
            const auto rcsId = convertU32Value(responseReader.readElementText());
            if (rcsId.has_value())
            {
                mConnectionInfo.rcsId = rcsId.value();
            }
            else
            {
                qCritical() << "Failed to read RcsID. Set to 0";
                mConnectionInfo.rcsId = 0;
            }
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("AVTransportID"))
        {
            const auto avTId = convertU32Value(responseReader.readElementText());
            if (avTId.has_value())
            {
                mConnectionInfo.avTransportId = avTId.value();
            }
            else
            {
                qCritical() << "Failed to read AVTransportID. Set to 0";
                mConnectionInfo.avTransportId = 0;
            }
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("ProtocolInfo"))
        {
            mConnectionInfo.protoclInfo = responseReader.readElementText();
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("PeerConnectionManager"))
        {
            mConnectionInfo.peerConnectionManager = responseReader.readElementText();
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("PeerConnectionId"))
        {
            const auto peerConId = convertU32Value(responseReader.readElementText());
            if (peerConId.has_value())
            {
                mConnectionInfo.peerConnectionId = peerConId.value();
            }
            else
            {
                qCritical() << "Failed to read PeerConnectionId. Set to 0";
                mConnectionInfo.avTransportId = 0;
            }
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Direction"))
        {
            const auto direction = convertDirection(responseReader.readElementText());
            if (direction.has_value())
            {
                mConnectionInfo.direction = direction.value();
            }
            else
            {
                qCritical() << "Failed to read Direction. Set to Input.";
                mConnectionInfo.direction = ConnectionInfoDirection::Input;
            }
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Status"))
        {
            const auto status = convertStatus(responseReader.readElementText());
            if (status.has_value())
            {
                mConnectionInfo.status = status.value();
            }
            else
            {
                qCritical() << "Failed to read Status Element. Set to unknown.";
                mConnectionInfo.status = ConnectionInfoStatus::Unknown;
            }
        }
    }

    if (responseReader.hasError())
    {
        qCritical() << "Failed GetCurrentConnectionInfo response";
        qCritical() << xmlResponse;
        qCritical() << "XML Error:" << responseReader.errorString();
    }
}

const ConnectionInfo &CurrentConnectionInfoResult::connectionInfo() const noexcept
{
    return mConnectionInfo;
}

std::optional<quint32> CurrentConnectionInfoResult::convertU32Value(QString const &rawValue) noexcept
{
    bool ok = false;
    const auto value = rawValue.toUInt(&ok);
    if (!ok)
    {
        return std::nullopt;
    }

    return value;
}

std::optional<ConnectionInfoDirection> CurrentConnectionInfoResult::convertDirection(QString const &rawValue) noexcept
{
    if (rawValue == QStringLiteral("Input"))
    {
        return ConnectionInfoDirection::Input;
    }
    else if (rawValue == QStringLiteral("Output"))
    {
        return ConnectionInfoDirection::Output;
    }

    return std::nullopt;
}

std::optional<ConnectionInfoStatus> CurrentConnectionInfoResult::convertStatus(QString const &rawValue) noexcept
{
    if (rawValue == QStringLiteral("OK"))
    {
        return ConnectionInfoStatus::OK;
    }
    else if (rawValue == QStringLiteral("Unknown"))
    {
        return ConnectionInfoStatus::Unknown;
    }
    else if (rawValue == QStringLiteral("ContentFormatMismatch"))
    {
        return ConnectionInfoStatus::ContentFormatMismatch;
    }
    else if (rawValue == QStringLiteral("InsufficientBandwidth"))
    {
        return ConnectionInfoStatus::InsufficientBandwidth;
    }
    else if (rawValue == QStringLiteral("UnreliableChannel"))
    {
        return ConnectionInfoStatus::UnreliableChannel;
    }

    return std::nullopt;
}

bool operator==(ConnectionInfo const &lhs, ConnectionInfo const &rhs)
{
    // clang-format off
    return (lhs.status == rhs.status) &&
           (lhs.direction == rhs.direction) &&
           (lhs.peerConnectionManager == rhs.peerConnectionManager) &&
           (lhs.protoclInfo == rhs.protoclInfo) &&
           (lhs.avTransportId == rhs.avTransportId) &&
           (lhs.rcsId == rhs.rcsId) &&
           (lhs.peerConnectionId == rhs.peerConnectionId);
    // clang-format on
}

bool operator!=(ConnectionInfo const &lhs, ConnectionInfo const &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
