// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetCurrentConnectionIdsResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"
#include <QDebug>
#include <QObject>

namespace UPnPAV
{

GetCurrentConnectionIdsResponse::GetCurrentConnectionIdsResponse(QString xmlResponse,
                                                                 ServiceControlPointDefinition scpd,
                                                                 SCPDAction action)
{
    auto reader = ResponseReader{xmlResponse, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const &elementName, QString &value, ResponseReader::ElementReadResult result) {
                         if (elementName == QStringLiteral("ConnectionIDs") and
                             result == ResponseReader::ElementReadResult::Ok) {
                             const auto idList = value.split(",");
                             for (auto const &rawId : std::as_const(idList)) {
                                 bool ok = false;
                                 const auto id = rawId.toInt(&ok, 10);
                                 if (ok) {
                                     mConnectionIds.push_back(id);
                                 } else {
                                     qCritical() << "Failed to convert connection id " << rawId << rawId;
                                 }
                             }
                         } else if (result == ResponseReader::ElementReadResult::ConversionError) {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         } else if (result == ResponseReader::ElementReadResult::Error) {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });
    auto const result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetCurrentConnectionInfo. Response was:" << reader.response();
        return;
    }
}

QVector<quint32> GetCurrentConnectionIdsResponse::currentConnectionIds() const noexcept
{
    return mConnectionIds;
}

} // namespace UPnPAV
