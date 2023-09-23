#pragma once
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

// clang-format off
constexpr auto ValidCurrentConnectionIdsResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
   <s:Body>
      <u:GetCurrentConnectionIDsResponse xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
         <CurrentConnectionIDs>12,13,52,12,16</CurrentConnectionIDs>
      </u:GetCurrentConnectionIDsResponse>
   </s:Body>
</s:Envelope>)";
// clang-format on
