// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetVolumeResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"

namespace UPnPAV
{

GetVolumeResponse::GetVolumeResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action)
{
    auto reader = ResponseReader{xmlResponse, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::readUnsignedInt16Value,
                     &reader,
                     [&](QString const& elementName, quint16 value, ResponseReader::ElementReadResult result) {
                         if (elementName == QStringLiteral("CurrentVolume") and
                             result == ResponseReader::ElementReadResult::Ok) {
                             mVolume = value;
                         } else if (result == ResponseReader::ElementReadResult::ConversionError) {
                             qCCritical(upnpavDevice) << "Failed to convert" << elementName;
                         } else if (result == ResponseReader::ElementReadResult::Error) {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });
    auto const result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetVolume. Response was:" << reader.response();
        qCCritical(upnpavDevice) << "Failed to read GetVolume. Error code:" << result;
        return;
    }
}

quint32 GetVolumeResponse::volume() const noexcept
{
    return mVolume;
}

} // namespace UPnPAV
