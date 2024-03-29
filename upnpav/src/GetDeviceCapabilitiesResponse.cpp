// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetDeviceCapabilitiesResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"

namespace UPnPAV
{

GetDeviceCapabilitiesResponse::GetDeviceCapabilitiesResponse(QString const& rawMsg,
                                                             ServiceControlPointDefinition scpd,
                                                             SCPDAction action)
    : d{new GetDeviceCapabilitiesResponseData{}}
{
    auto reader = ResponseReader{rawMsg, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const& elementName, QString value, ResponseReader::ElementReadResult result) {
                         const bool ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("PlayMedia") && ok) {
                             d->mPlayMedia = value.remove(" ").split(",");
                         }
                         if (elementName == QStringLiteral("RecMedia") && ok) {
                             d->mRecMedia = value.remove(" ").split(",");
                         }
                         if (elementName == QStringLiteral("RecQualityModes") && ok) {
                             d->mRecQModes = value.remove(" ").split(",");
                         } else if (not ok) {
                             qCCritical(upnpavDevice)
                                 << "Failed to read element" << elementName << "Error:" << static_cast<qint32>(result);
                         }
                     });

    auto const result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetDeviceCapabilities response Error:"
                                 << static_cast<qint32>(result) << "Response:" << rawMsg;
        return;
    }
}

QStringList const& GetDeviceCapabilitiesResponse::playMedia() const noexcept
{
    return d->mPlayMedia;
}

QStringList const& GetDeviceCapabilitiesResponse::recMedia() const noexcept
{
    return d->mRecMedia;
}

QStringList const& GetDeviceCapabilitiesResponse::recQualityModes() const noexcept
{
    return d->mRecQModes;
}

} // namespace UPnPAV
