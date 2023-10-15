// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetDeviceCapabilitiesResponse.h"
#include "private/LoggingCategories.h"
#include "private/ResponseReader.h"

namespace UPnPAV
{

GetDeviceCapabilitiesResponse::GetDeviceCapabilitiesResponse(QString const &rawMsg,
                                                             ServiceControlPointDefinition scpd,
                                                             SCPDAction action)
    : d{new GetDeviceCapabilitiesResponseData{}}
{
    auto reader = ResponseReader{rawMsg, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const &elementName, QString value, ResponseReader::ElementReadResult result) {
                         const bool ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("PlayMedia") && ok)
                         {
                             d->mPlayMedia = value.remove(" ").split(",");
                         }
                         if (elementName == QStringLiteral("RecMedia") && ok)
                         {
                             d->mRecMedia = value.remove(" ").split(",");
                         }
                         if (elementName == QStringLiteral("RecQualityModes") && ok)
                         {
                             d->mRecQModes = value.remove(" ").split(",");
                         }
                         else if (not ok)
                         {
                             qCCritical(upnpavDevice)
                                 << "Failed to read element" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok)
    {
        qCCritical(upnpavDevice) << "Failed to read GetDeviceCapabilities response Error:"
                                 << static_cast<qint32>(result) << "Response:" << rawMsg;
        return;
    }
}

const QStringList &GetDeviceCapabilitiesResponse::playMedia() const noexcept
{
    return d->mPlayMedia;
}

const QStringList &GetDeviceCapabilitiesResponse::recMedia() const noexcept
{
    return d->mRecMedia;
}

const QStringList &GetDeviceCapabilitiesResponse::recQualityModes() const noexcept
{
    return d->mRecQModes;
}

} // namespace UPnPAV
