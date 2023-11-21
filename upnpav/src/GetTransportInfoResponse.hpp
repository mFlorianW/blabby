// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>

namespace UPnPAV
{
struct GetTransportInfoResponseData;
class BLABBYUPNPAV_EXPORT GetTransportInfoResponse final
{
public:
    enum class TransportState : quint8
    {
        Stopped, ///< The playback is stopped
        PausedPlayback, ///< The playback is paused
        PausedRecording, ///< The recording is paused
        Playing, ///< The playback is active
        Recording, ///< The recording is active
        Transitioning, ///< The transition to the next state is active
        NoMediaPresent, ///< Idle state
    };

    enum class TransportStatus : quint8
    {
        Ok, ///< Everything ok
        ErrorOccured, ///< Some error occured
        Unknown ///< Unsupported string received, e.g. vendor defined.
    };

    /**
     * Creates an instance of GetTransportInfoResponse
     * @param rawMessage The XML response message of the call.
     * @param scpd The service control point defintion which is needed to read the response.
     * @param action The action of the call which is also needed to read the response.
     */
    GetTransportInfoResponse(QString rawMessage, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the current transport state of.
     * If it failed to convert the state out of the response.
     * message TransportState::NoMediaPresent will be returned.
     * @return The value of the CurrentTransportState field of the message.
     */
    TransportState currentTransportState() const noexcept;

    /**
     * Gives the current transport status of the response.
     * It failed to convert the status or vendor defined value ist the is set to TransportStatus::Unknown.
     * @return The value of the CurrentTransportStatus field of the message.
     */
    TransportStatus currentTransportStatus() const noexcept;

    /**
     * Gives the transport play speed of the response.
     * @return The value of the TransportPlaySpeed field of the message.
     */
    QString const &transportPlaySpeed() const noexcept;

private:
    static std::optional<TransportState> convertTransportState(QString const &rawString);
    static TransportStatus convertTransportStatus(QString const &rawString);

private:
    QExplicitlySharedDataPointer<GetTransportInfoResponseData> d;
};

struct GetTransportInfoResponseData : public QSharedData
{
    GetTransportInfoResponse::TransportState mCurrentTransportState{
        GetTransportInfoResponse::TransportState::NoMediaPresent};
    GetTransportInfoResponse::TransportStatus mCurrentTransportStatus{
        GetTransportInfoResponse::TransportStatus::Unknown};
    QString mTransportPlaySpeed{""};
};

} // namespace UPnPAV
