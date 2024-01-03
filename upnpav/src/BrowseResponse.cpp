// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BrowseResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"
#include <QXmlStreamReader>

namespace UPnPAV
{

BrowseResponse::BrowseResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action)
{
    auto reader = ResponseReader{xmlResponse, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::unsignedIntValueRead,
                     &reader,
                     [&](QString const &elementName, quint32 value, ResponseReader::ElementReadResult result) {
                         bool conversionCorrect = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("NumberReturned") and conversionCorrect) {
                             mNumberReturned = value;
                         } else if (elementName == QStringLiteral("TotalMatches") and conversionCorrect) {
                             mTotalMatches = value;
                         } else if (elementName == QStringLiteral("UpdateID") and conversionCorrect) {
                             mUpdateId = value;
                         } else if (result == ResponseReader::ElementReadResult::ConversionError) {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         } else if (result == ResponseReader::ElementReadResult::Error) {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });

    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const &elementName, QString &value, ResponseReader::ElementReadResult result) {
                         if (elementName == QStringLiteral("Result") and
                             result == ResponseReader::ElementReadResult::Ok) {
                             m_objects = MediaServerObject::createFromDidl(value);
                         } else if (result == ResponseReader::ElementReadResult::ConversionError) {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         } else if (result == ResponseReader::ElementReadResult::Error) {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetCurrentConnectionInfo. Response was:" << reader.response();
        return;
    }
}

quint32 BrowseResponse::totalMatches() const noexcept
{
    return mTotalMatches;
}

quint32 BrowseResponse::numberReturned() const noexcept
{
    return mNumberReturned;
}

quint32 BrowseResponse::updateId() const noexcept
{
    return mUpdateId;
}

const QVector<MediaServerObject> &BrowseResponse::objects() const noexcept
{
    return m_objects;
}

} // namespace UPnPAV
