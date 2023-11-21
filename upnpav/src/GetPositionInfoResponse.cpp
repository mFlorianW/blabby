// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "GetPositionInfoResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"

namespace UPnPAV
{

GetPositionInfoResponse::GetPositionInfoResponse(QString rawMessage,
                                                 ServiceControlPointDefinition scpd,
                                                 SCPDAction action)
    : d{new GetPositionInfoResponseData{}}
{
    auto reader = ResponseReader{rawMessage, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::unsignedIntValueRead,
                     &reader,
                     [&](QString const &elementName, quint32 value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("Track") && ok)
                         {
                             d->mTrack = value;
                         }
                         else if (!ok)
                         {
                             qCCritical(upnpavDevice)
                                 << "Failed to read" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });
    QObject::connect(
        &reader,
        &ResponseReader::stringValueRead,
        &reader,
        [&](QString const &elementName, QString value, ResponseReader::ElementReadResult result) {
            const auto ok = result == ResponseReader::ElementReadResult::Ok;
            if (elementName == QStringLiteral("TrackDuration") && ok)
            {
                auto time = QTime::fromString(value, "h:m:s.z");
                d->mTrackDuration = time.isValid() ? time : QTime{0, 0, 0, 0};
            }
            else if (elementName == QStringLiteral("TrackMetaData") && ok)
            {
                d->mTrackMetaData = value;
            }
            else if (elementName == QStringLiteral("TrackURI") && ok)
            {
                d->mTrackUri = value;
            }
            if (elementName == QStringLiteral("RelTime") && value != QStringLiteral("NOT_IMPLEMENTED") && ok)
            {
                const auto time = converToTime(value);
                d->mRelTime = time.isValid() ? time : QTime{0, 0, 0, 0};
            }
            if (elementName == QStringLiteral("AbsTime") && value != QStringLiteral("NOT_IMPLEMENTED") && ok)
            {
                const auto time = converToTime(value);
                d->mAbsTime = time.isValid() ? time : QTime{0, 0, 0, 0};
            }
            else if (!ok)
            {
                qCCritical(upnpavDevice) << "Failed to read" << elementName << "Error:" << static_cast<qint32>(result);
            }
        });
    QObject::connect(&reader,
                     &ResponseReader::signedIntValueRead,
                     &reader,
                     [&](QString const &elementName, qint32 value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("RelCount") && ok)
                         {
                             d->mRelCount = value;
                         }
                         if (elementName == QStringLiteral("AbsCount") && ok)
                         {
                             d->mAbsCount = value;
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
        qCCritical(upnpavDevice) << "Failed to read GetCurrentConnectionInfo. Response was:" << reader.response();
        return;
    }
}

quint32 GetPositionInfoResponse::track() const noexcept
{
    return d->mTrack;
}

const QTime &GetPositionInfoResponse::trackDuration() const noexcept
{
    return d->mTrackDuration;
}

const QString &GetPositionInfoResponse::trackMetaData() const noexcept
{
    return d->mTrackMetaData;
}

const QString &GetPositionInfoResponse::trackUri() const noexcept
{
    return d->mTrackUri;
}

const QTime &GetPositionInfoResponse::relTime() const noexcept
{
    return d->mRelTime;
}

const QTime &GetPositionInfoResponse::absTime() const noexcept
{
    return d->mAbsTime;
}

qint32 GetPositionInfoResponse::relCount() const noexcept
{
    return d->mRelCount;
}

qint32 GetPositionInfoResponse::absCount() const noexcept
{
    return d->mAbsCount;
}

QTime GetPositionInfoResponse::converToTime(QString const &rawMsg)
{
    return QTime::fromString(rawMsg, "h:m:s.z");
}

} // namespace UPnPAV
