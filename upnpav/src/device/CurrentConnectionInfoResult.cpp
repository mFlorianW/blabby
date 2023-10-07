// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "CurrentConnectionInfoResult.h"
#include "private/LoggingCategories.h"
#include "private/ResponseReader.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <optional>

namespace UPnPAV
{

CurrentConnectionInfoResult::CurrentConnectionInfoResult(QString const &xmlResponse,
                                                         ServiceControlPointDefinition const &scpd,
                                                         SCPDAction action)
{
    auto resultReader = ResponseReader(xmlResponse, scpd, action);
    QObject::connect(&resultReader,
                     &ResponseReader::stringValueRead,
                     &resultReader,
                     [&](QString const &elementName, QString const &value, ResponseReader::ElementReadResult result) {
                         const auto conversionOk = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("Direction") and conversionOk)
                         {
                             const auto direction = convertDirection(value);
                             if (direction.has_value())
                             {
                                 mConnectionInfo.direction = direction.value();
                             }
                             else
                             {
                                 qCCritical(upnpavDevice) << "Failed to read Direction. Set to Input.";
                                 mConnectionInfo.direction = ConnectionInfoDirection::Unknown;
                             }
                         }
                         else if (elementName == QStringLiteral("ProtocolInfo") and conversionOk)
                         {
                             mConnectionInfo.protoclInfo = value;
                         }
                         else if (elementName == QStringLiteral("PeerConnectionManager") and conversionOk)
                         {
                             mConnectionInfo.peerConnectionManager = value;
                         }
                         else if (elementName == QStringLiteral("Status") and conversionOk)
                         {
                             const auto status = convertStatus(value);
                             if (status.has_value())
                             {
                                 mConnectionInfo.status = status.value();
                             }
                             else
                             {
                                 qCCritical(upnpavDevice) << "Failed to read Status Element. Set to unknown.";
                                 mConnectionInfo.status = ConnectionInfoStatus::Unknown;
                             }
                         }
                         else if (result == ResponseReader::ElementReadResult::ConversionError)
                         {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         }
                         else if (result == ResponseReader::ElementReadResult::Error)
                         {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });
    QObject::connect(&resultReader,
                     &ResponseReader::signedIntValueRead,
                     &resultReader,
                     [&](QString const &elementName, quint32 value, ResponseReader::ElementReadResult result) {
                         const auto conversionOk = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("RcsID") and conversionOk)
                         {
                             mConnectionInfo.rcsId = value;
                         }
                         else if (elementName == QStringLiteral("AVTransportID") and conversionOk)
                         {
                             mConnectionInfo.avTransportId = value;
                         }
                         else if (elementName == QStringLiteral("PeerConnectionID") and conversionOk)
                         {
                             mConnectionInfo.peerConnectionId = value;
                         }
                         else if (result == ResponseReader::ElementReadResult::ConversionError)
                         {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         }
                         else if (result == ResponseReader::ElementReadResult::Error)
                         {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });
    const auto result = resultReader.read();
    if (result != ResponseReader::ReadResult::Ok)
    {
        qCritical() << "Failed to read GetCurrentConnectionInfo. Response was:" << resultReader.response();
        return;
    }
}

CurrentConnectionInfoResult::~CurrentConnectionInfoResult() = default;

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
