// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetTransportInfoResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"
#include <QObject>

namespace UPnPAV
{

GetTransportInfoResponse::GetTransportInfoResponse(QString rawMessage,
                                                   ServiceControlPointDefinition scpd,
                                                   SCPDAction action)
    : d{new GetTransportInfoResponseData{}}
{
    auto reader = ResponseReader{rawMessage, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const &elementName, QString value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("CurrentTransportState") && ok)
                         {
                             d->mCurrentTransportState =
                                 convertTransportState(value).value_or(TransportState::NoMediaPresent);
                         }
                         else if (elementName == QStringLiteral("CurrentTransportStatus") && ok)
                         {
                             d->mCurrentTransportStatus = convertTransportStatus(value);
                         }
                         else if (elementName == QStringLiteral("CurrentSpeed") && ok)
                         {
                             d->mTransportPlaySpeed = value;
                         }
                         else if (!ok)
                         {
                             qCCritical(upnpavDevice)
                                 << "Failed to read" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok)
    {
        qCCritical(upnpavDevice) << "Failed to read GetTransportInfoResponse. Response was:" << reader.response();
        return;
    }
}

GetTransportInfoResponse::TransportState GetTransportInfoResponse::currentTransportState() const noexcept
{
    return d->mCurrentTransportState;
}

GetTransportInfoResponse::TransportStatus GetTransportInfoResponse::currentTransportStatus() const noexcept
{
    return d->mCurrentTransportStatus;
}

const QString &GetTransportInfoResponse::transportPlaySpeed() const noexcept
{
    return d->mTransportPlaySpeed;
}

std::optional<GetTransportInfoResponse::TransportState> GetTransportInfoResponse::convertTransportState(
    QString const &rawString)
{
    if (rawString == QStringLiteral("STOPPED"))
    {
        return TransportState::Stopped;
    }
    else if (rawString == QStringLiteral("PAUSED_PLAYBACK"))
    {
        return TransportState::PausedPlayback;
    }
    else if (rawString == QStringLiteral("PAUSED_RECORDING"))
    {
        return TransportState::PausedRecording;
    }
    else if (rawString == QStringLiteral("PLAYING"))
    {
        return TransportState::Playing;
    }
    else if (rawString == QStringLiteral("RECORDING"))
    {
        return TransportState::Recording;
    }
    else if (rawString == QStringLiteral("TRANSITIONING"))
    {
        return TransportState::Transitioning;
    }
    else if (rawString == QStringLiteral("NO_MEDIA_PRESENT"))
    {
        return TransportState::NoMediaPresent;
    }
    else
    {
        qCCritical(upnpavDevice) << "Failed to convert transport state:" << rawString;
        return std::nullopt;
    }
}

GetTransportInfoResponse::TransportStatus GetTransportInfoResponse::convertTransportStatus(QString const &rawString)
{

    if (rawString == QStringLiteral("OK"))
    {
        return TransportStatus::Ok;
    }
    else if (rawString == QStringLiteral("ERROR_OCCURED"))
    {
        return TransportStatus::ErrorOccured;
    }
    else
    {
        return TransportStatus::Unknown;
    }
}

} // namespace UPnPAV
