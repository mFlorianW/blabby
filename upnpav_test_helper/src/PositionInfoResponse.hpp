// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QString>

namespace UPnPAV
{

/**
 * Gives the response of a GetPositionInfo call.
 * @param trackUri The URI of the track.
 * @param trackMetaData The DIDL-Lite metadata of the track, it is escaped.
 * @param trackDuration The duration of the track, e.g. 0:04:31.
 * @param relTime The position in the track, e.g. 0:01:42.
 */
inline QString positionInfoResponse(QString const& trackUri,
                                    QString const& trackMetaData = QStringLiteral("NOT_IMPLEMENTED"),
                                    QString const& trackDuration = QStringLiteral("0:00:00"),
                                    QString const& relTime = QStringLiteral("0:00:00"))
{
    constexpr auto response = R"(<?xml version="1.0" encoding="UTF-8"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
<s:Body>
<u:GetPositionInfoResponse xmlns:u="urn:schemas-upnp-org:service:AVTransport:1">
<Track>1</Track>
<TrackDuration>%1</TrackDuration>
<TrackMetaData>%2</TrackMetaData>
<TrackURI>%3</TrackURI>
<RelTime>%4</RelTime>
<AbsTime>NOT_IMPLEMENTED</AbsTime>
<RelCount>2147483647</RelCount>
<AbsCount>2147483647</AbsCount>
</u:GetPositionInfoResponse>
</s:Body>
</s:Envelope>)";
    return QString{response}.arg(trackDuration, trackMetaData.toHtmlEscaped(), trackUri.toHtmlEscaped(), relTime);
}

} // namespace UPnPAV
