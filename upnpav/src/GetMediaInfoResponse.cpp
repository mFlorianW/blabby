// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetMediaInfoResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"
#include <QObject>

namespace UPnPAV
{

GetMediaInfoResponse::GetMediaInfoResponse(QString rawMsg, ServiceControlPointDefinition scpd, SCPDAction action)
    : d{new GetMediaInfoResponseData{}}
{
    auto reader = ResponseReader{rawMsg, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::unsignedIntValueRead,
                     &reader,
                     [&](QString const& elementName, quint32 value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("NrTracks") && ok) {
                             d->mNumberOfTracks = value;
                         } else if (!ok) {
                             qCCritical(upnpavDevice)
                                 << "Failed to read" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const& elementName, QString value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("MediaDuration") && ok) {
                             d->mMediaDuration = QTime::fromString(value, "hh:mm:ss.zzz");
                         } else if (elementName == QStringLiteral("CurrentURI") && ok) {
                             d->mCurrentUri = value;
                         } else if (elementName == QStringLiteral("CurrentURIMetaData") && ok) {
                             d->mCurrentMetaData = value;
                         } else if (elementName == QStringLiteral("NextURI") && ok) {
                             d->mNextUri = value;
                         } else if (elementName == QStringLiteral("NextURIMetaData") && ok) {
                             d->mNextUriMetaData = value;
                         } else if (elementName == QStringLiteral("PlayMedium") && ok) {
                             d->mPlayMedium = value;
                         } else if (elementName == QStringLiteral("RecordMedium") && ok) {
                             d->mRecordMedium = value;
                         } else if (elementName == QStringLiteral("WriteStatus") && ok) {
                             d->mWriteStatus = value;
                         } else if (!ok) {
                             qCCritical(upnpavDevice)
                                 << "Failed to read" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });

    auto const result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetCurrentConnectionInfo. Response was:" << reader.response();
        return;
    }
}

quint32 GetMediaInfoResponse::numberOfTracks() const noexcept
{
    return d->mNumberOfTracks;
}

std::optional<QTime> GetMediaInfoResponse::mediaDuration() const noexcept
{
    if (d->mMediaDuration.has_value() && d->mMediaDuration.value_or(QTime{}).isValid()) {
        return d->mMediaDuration;
    }

    return std::nullopt;
}

QString const& GetMediaInfoResponse::currentUri() const noexcept
{
    return d->mCurrentUri;
}

QString const& GetMediaInfoResponse::currentUriMetaData() const noexcept
{
    return d->mCurrentMetaData;
}

QString const& GetMediaInfoResponse::nextUri() const noexcept
{
    return d->mNextUri;
}

QString const& GetMediaInfoResponse::nextUriMetaData() const noexcept
{
    return d->mNextUriMetaData;
}

QString const& GetMediaInfoResponse::playMedium() const noexcept
{
    return d->mPlayMedium;
}

QString const& GetMediaInfoResponse::recordMedium() const noexcept
{
    return d->mRecordMedium;
}

QString const& GetMediaInfoResponse::writeStatus() const noexcept
{
    return d->mWriteStatus;
}
} // namespace UPnPAV
