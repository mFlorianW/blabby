/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef RESPONSE_H
#define RESPONSE_H

constexpr char xmlResponse[] =
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

constexpr char didlOnlyOneContainer[] =
{
    "&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
        "&lt;container id=&quot;1&quot; parentID=&quot;0&quot; restricted=&quot;false&quot;&gt;"
            "&lt;dc:title&gt;MyMusic&lt;/dc:title&gt;"
            "&lt;upnp:class&gt;object.container.storageFolder&lt;/upnp:class&gt;"
        "&lt;/container&gt;"
    "&lt;/DIDLE-Lite&gt;"
};

#endif // RESPONSE_H
