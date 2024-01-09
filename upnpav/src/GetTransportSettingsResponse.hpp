// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>

namespace UPnPAV
{

struct GetTransportSettingsResponseData;
class BLABBYUPNPAV_EXPORT GetTransportSettingsResponse final
{
public:
    /**
     * The current play modes of the device.
     */
    enum class PlayMode
    {
        Normal,
        Shuffle,
        RepeatOne,
        RepeatAll,
        Random,
        DirectOne,
        Intro
    };
    /**
     * Creates an instance of the GetTransportSettingsResponse
     * @param rawMsg The XML response of the call.
     * @param scpd The service control point defintion as defined by the device.
     * @param action The service control point definition action for that call.
     */
    GetTransportSettingsResponse(QString rawMsg, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the current play mode of the device.
     * @return The value of the PlayMode field of the response.
     */
    PlayMode playMode() const noexcept;

    /**
     * Gives the current record mode of the device.
     * If the device doesn't support recording, "NOT_IMPLEMENTED" will be returned.
     * @return The value of the RecQualityMode field of the response.
     */
    QString const& recQualityMode() const noexcept;

private:
    static std::optional<PlayMode> convertPlayMode(QString const& rawMode) noexcept;

private:
    QExplicitlySharedDataPointer<GetTransportSettingsResponseData> d;
};

struct GetTransportSettingsResponseData : public QSharedData
{
    GetTransportSettingsResponse::PlayMode mPlayMode{GetTransportSettingsResponse::PlayMode::Normal};
    QString mRecQMode{"NOT_IMPLEMENTED"};
};

} // namespace UPnPAV
