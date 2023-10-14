// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "ServiceControlPointDefinition.h"
#include <QExplicitlySharedDataPointer>
#include <QTime>

namespace UPnPAV
{
struct GetPositionInfoResponseData;
class GetPositionInfoResponse
{
public:
    /**
     * Creates an instance of GetPositionInfoResponse
     * @param rawMessage The XML response message of the call.
     * @param scpd The service control point defintion which is needed to read the response.
     * @param action The action of the call which is also needed to read the response.
     */
    GetPositionInfoResponse(QString rawMessage, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the currently selected track.
     * The value is zero when no track is selected or is not supported.
     * @return The value of the Track field of the response.
     */
    quint32 track() const noexcept;

    /**
     * Gives the track duration.
     * If the AVTransport Servic does not support the 00:00:00.000 will be returned.
     * @return The value of the TrackDuration field of the response.
     */
    QTime const &trackDuration() const noexcept;

    /**
     * Gives the track meta data.
     * If the AVTransport Services does not support it, "NOT_IMPLEMENTED" will be returned.
     * @return The value of the TrackMetaData field of the response.
     */
    QString const &trackMetaData() const noexcept;

    /**
     * Gives the track URI.
     * @return The value of the TrackURI field of the response.
     */
    QString const &trackUri() const noexcept;

    /**
     * Gives the current position in terms of time of the current track.
     * If the AVTransport doesn't support it 00:00:00.000 will be returned.
     * @return The value of the RelTime field of the response.
     */
    QTime const &relTime() const noexcept;

    /**
     * Gives the absolute position in terms of time of the media.
     * If the AVTransport doesn't support it 00:00:00.000 will be returned.
     * @return The value of the AbsTime field of the response.
     */
    QTime const &absTime() const noexcept;

    /**
     * Gives the current position in terms of dimensionless counter from the start of the current track.
     * If the AVTransport service doesn't support it INT_MAX will be returned.
     * @return The value of the RelCount field of the response.
     */
    qint32 relCount() const noexcept;

    /**
     * Gives the current position in terms of dimensionless counter from the start of the loaded media.
     * If the AVTransport service doesn't support it INT_MAX will be returned.
     * @return The value of the RelCount field of the response.
     */
    qint32 absCount() const noexcept;

private:
    static inline QTime converToTime(QString const &rawMsg);

private:
    QExplicitlySharedDataPointer<GetPositionInfoResponseData> d;
};

struct GetPositionInfoResponseData : public QSharedData
{
    quint32 mTrack{0};
    QTime mTrackDuration{0, 0, 0, 0};
    QString mTrackMetaData{""};
    QString mTrackUri{""};
    QTime mRelTime{0, 0, 0, 0};
    QTime mAbsTime{0, 0, 0, 0};
    qint32 mRelCount{0};
    qint32 mAbsCount{0};
};

} // namespace UPnPAV
