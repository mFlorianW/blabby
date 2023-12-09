// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

// clang-format off
namespace UPnPAV
{

constexpr char const *xmlResponse =
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

constexpr char const* didlOnlyOneItem = {
    "&lt;DIDL-Lite xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\"&gt;"
    "&lt;item id=\"64$0$0\" parentID=\"64$0\" restricted=\"1\"&gt;&lt;dc:title&gt;Dekmantel Boiler Room 2016 -&lt;/dc:title&gt;&lt;upnp:class&gt;object.item.audioItem.musicTrack&lt;/upnp:class&gt;&lt;dc:description&gt;blub&lt;/dc:description&gt;&lt;dc:creator&gt;Ben Klock&lt;/dc:creator&gt;&lt;upnp:artist&gt;Ben Klock&lt;/upnp:artist&gt;&lt;res size=\"124814567\" duration=\"1:26:40.436\" bitrate=\"192000\" sampleFrequency=\"44100\" nrAudioChannels=\"2\" protocolInfo=\"http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000\"&gt;http://192.168.0.2:8200/MediaItems/23.mp3&lt;/res&gt;&lt;/item&gt;&lt;/DIDL-Lite&gt;"
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

constexpr auto realWorldResponse = R"(
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
   <s:Body>
      <u:BrowseResponse xmlns:u="urn:schemas-upnp-org:service:ContentDirectory:1">
         <Result>&lt;DIDL-Lite xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:upnp="urn:schemas-upnp-org:metadata-1-0/upnp/" xmlns="urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/" xmlns:dlna="urn:schemas-dlna-org:metadata-1-0/"&gt;
&lt;container id="64" parentID="0" restricted="1" searchable="1" childCount="4"&gt;&lt;dc:title&gt;Browse Folders&lt;/dc:title&gt;&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;&lt;upnp:storageUsed&gt;-1&lt;/upnp:storageUsed&gt;&lt;/container&gt;&lt;container id="1" parentID="0" restricted="1" searchable="1" childCount="7"&gt;&lt;dc:title&gt;Music&lt;/dc:title&gt;&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;&lt;upnp:storageUsed&gt;-1&lt;/upnp:storageUsed&gt;&lt;/container&gt;&lt;container id="3" parentID="0" restricted="1" searchable="1" childCount="5"&gt;&lt;dc:title&gt;Pictures&lt;/dc:title&gt;&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;&lt;upnp:storageUsed&gt;-1&lt;/upnp:storageUsed&gt;&lt;/container&gt;&lt;container id="2" parentID="0" restricted="1" searchable="1" childCount="3"&gt;&lt;dc:title&gt;Video&lt;/dc:title&gt;&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;&lt;upnp:storageUsed&gt;-1&lt;/upnp:storageUsed&gt;&lt;/container&gt;&lt;/DIDL-Lite&gt;</Result>
         <NumberReturned>4</NumberReturned>
         <TotalMatches>4</TotalMatches>
         <UpdateID>2</UpdateID>
      </u:BrowseResponse>
   </s:Body>
</s:Envelope>)";

}
// clang-format on
