// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>
#include <QTime>
#include <optional>

namespace UPnPAV
{
struct GetMediaInfoResponseData;
class BLABBYUPNPAV_EXPORT GetMediaInfoResponse final
{
public:
    /**
     * Creates an instance of the GetMediaInfoResponse
     */
    GetMediaInfoResponse(QString rawMsg, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the number of tracks controlled by the AVTransport instance.
     * @return The value of the NumberOfTracks field in the response.
     */
    quint32 numberOfTracks() const noexcept;

    /**
     * The duration of the current media that is currently played.
     * If the value can't be converted or no media is playing a nullopt is returned.
     * @return The value of the MediaDuration field in the response.
     */
    std::optional<QTime> mediaDuration() const noexcept;

    /**
     * The URI of the resource that is controlled by the AVTransport instance.
     * @return The value of the CurrentURI field of the response.
     */
    QString const &currentUri() const noexcept;

    /**
     * The meta data of the resource that is controlled by the AVTransport instance.
     * If the service doesn't support that feature NOT_IMPLEMENTED is returned.
     * @return The value of the CurrentURIMeta field of the response.
     */
    QString const &currentUriMetaData() const noexcept;

    /**
     * The URI of the resource that is controlled by the AVTransport instance and is played next
     * @return The value of the NextURI field of the response.
     */
    QString const &nextUri() const noexcept;

    /**
     * The meta data of the resource that is controlled by the AVTransport instance and is played next.
     * If the service doesn't support that feature "NOT_IMPLEMENTED" is returned.
     * @return The value of the NextURIMeta field of the response.
     */
    QString const &nextUriMetaData() const noexcept;

    /**
     * The storage medium of the resource specified by the AVTransportURI.
     * @return The value of the PlayMedium field of the response.
     */
    QString const &playMedium() const noexcept;

    /**
     * The storage medium of the resource specified by the AVTransportURI for a recording action.
     * If the implemented service has no support the "NOT_IMPLEMENTED" is returned.
     * @return The value of the RecordMedium field of the response.
     */
    QString const &recordMedium() const noexcept;

    /**
     * Gives the write protection status of the media.
     * "NOT_WRITEABLE" indicates a read only medium like, DVD or CD.
     * If the implemented service has no support for it, "NOT_IMPLEMENTED" is returned.
     * @return The value of the WriteStatus field of the response.
     */
    QString const &writeStatus() const noexcept;

private:
    QExplicitlySharedDataPointer<GetMediaInfoResponseData> d;
};

struct GetMediaInfoResponseData : public QSharedData
{
    quint32 mNumberOfTracks{0};
    std::optional<QTime> mMediaDuration{std::nullopt};
    QString mCurrentUri{""};
    QString mCurrentMetaData{""};
    QString mNextUri{""};
    QString mNextUriMetaData{""};
    QString mPlayMedium{""};
    QString mRecordMedium{""};
    QString mWriteStatus{""};
};

} // namespace UPnPAV
