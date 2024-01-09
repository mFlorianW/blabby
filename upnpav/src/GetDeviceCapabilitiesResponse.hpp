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
struct GetDeviceCapabilitiesResponseData;
class BLABBYUPNPAV_EXPORT GetDeviceCapabilitiesResponse final
{
public:
    /**
     * Creates an instance of GetDeviceCapabilitiesResponse
     * @param rawMsg The XML response of the call.
     * @param scpd The service control point definition of the device.
     * @param action The SCPD action of the AVTransport service.
     */
    GetDeviceCapabilitiesResponse(QString const& rawMsg, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the list of media that the device can play
     * @return The value of the PlayMedia field of the response.
     */
    QStringList const& playMedia() const noexcept;

    /**
     * Gives the list of record media that the device can record.
     * If the device can't record anything, then "NOT_IMPLMENTED" will be returned.
     * @return The value of the RecMedia field of the response.
     */
    QStringList const& recMedia() const noexcept;

    /**
     * Gives the list of the possible recording quality modes.
     * If the device can't record anything, the "NOT_IMPLMENTED" will be returned.
     * @return The value of the RecQualityModes field of the response.
     */
    QStringList const& recQualityModes() const noexcept;

private:
    QExplicitlySharedDataPointer<GetDeviceCapabilitiesResponseData> d;
};

struct GetDeviceCapabilitiesResponseData : public QSharedData
{
    QStringList mPlayMedia;
    QStringList mRecMedia;
    QStringList mRecQModes;
};

} // namespace UPnPAV
