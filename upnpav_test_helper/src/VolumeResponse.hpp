// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

namespace UPnPAV
{

// clang-format off
constexpr auto ValidGetVolumeResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
   <s:Body>
      <u:GetVolumeResponse xmlns:u="urn:schemas-upnp-org:service:RenderingControl:1">
         <CurrentVolume>98</CurrentVolume>
      </u:GetVolumeResponse>
   </s:Body>
</s:Envelope>)";
// clang-format on

} // namespace UPnPAV
