// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QString>

namespace UPnPAV
{

/**
 * Gives the response of a GetMute call.
 * @param currentMute The raw value of CurrentMute, e.g. 1 or false.
 */
inline QString getMuteResponse(QString const& currentMute)
{
    constexpr auto response = R"(<?xml version="1.0" encoding="UTF-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
   <s:Body>
      <u:GetMuteResponse xmlns:u="urn:schemas-upnp-org:service:RenderingControl:1">
         <CurrentMute>%1</CurrentMute>
      </u:GetMuteResponse>
   </s:Body>
</s:Envelope>)";
    return QString{response}.arg(currentMute);
}

} // namespace UPnPAV
