// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// clang-format off

constexpr char const* xmlResponse =
{
    "<?xml version=\"1.0\"?>"
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
    " s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
        "<s:Body>"
        "<u:Browse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\">"
            "<Result>%1</Result>"
            "<NumberReturned>%2</NumberReturned>"
            "<TotalMatches>%3</TotalMatches>"
            "<UpdateID>%4</UpdateID>"
        "</u:Browse>"
    "</s:Body>"
    "</s:Envelope>"
};

constexpr char const* didlOnlyOneContainer =
{
    "&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
        "&lt;container id=&quot;1&quot; parentID=&quot;0&quot; restricted=&quot;false&quot;&gt;"
            "&lt;dc:title&gt;MyMusic&lt;/dc:title&gt;"
            "&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;"
        "&lt;/container&gt;"
    "&lt;/DIDLE-Lite&gt;"
};

constexpr char const* didlOnlyTwoContainer =
{
    "&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
        "&lt;container id=&quot;1&quot; parentID=&quot;0&quot; restricted=&quot;false&quot;&gt;"
            "&lt;dc:title&gt;MyMusic0&lt;/dc:title&gt;"
            "&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;"
        "&lt;/container&gt;"
        "&lt;container id=&quot;2&quot; parentID=&quot;0&quot; restricted=&quot;false&quot;&gt;"
            "&lt;dc:title&gt;MyMusic1&lt;/dc:title&gt;"
        "&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;"
    "&lt;/container&gt;"
    "&lt;/DIDLE-Lite&gt;"
};

// clang-format on
