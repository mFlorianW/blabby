// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetMuteResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"

namespace UPnPAV
{

GetMuteResponse::GetMuteResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action)
{
    auto reader = ResponseReader{xmlResponse, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::boolValueRead,
                     &reader,
                     [&](QString const& elementName, bool value, ResponseReader::ElementReadResult result) {
                         if (elementName == QStringLiteral("CurrentMute") and
                             result == ResponseReader::ElementReadResult::Ok) {
                             mMute = value;
                         } else if (result != ResponseReader::ElementReadResult::Ok) {
                             qCCritical(upnpavDevice) << "Failed to convert" << elementName;
                         }
                     });
    auto const result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetMute. Response was:" << reader.response();
    }
}

bool GetMuteResponse::mute() const noexcept
{
    return mMute;
}

} // namespace UPnPAV
