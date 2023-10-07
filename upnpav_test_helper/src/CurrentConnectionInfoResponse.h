// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

namespace UPnPAV
{

// clang-format off
constexpr auto ValidCurrentConnectionInfoResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
   <s:Body>
      <u:GetCurrentConnectionInfoResponse xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
         <RcsID>123</RcsID>
         <AVTransportID>12</AVTransportID>
         <ProtocolInfo>audio/mpeg</ProtocolInfo>
         <PeerConnectionManager>DummyText</PeerConnectionManager>
         <PeerConnectionID>321</PeerConnectionID>
         <Direction>Input</Direction>
         <Status>OK</Status>
      </u:GetCurrentConnectionInfoResponse>
   </s:Body>
</s:Envelope>)";
// clang-format on

} // namespace UPnPAV
