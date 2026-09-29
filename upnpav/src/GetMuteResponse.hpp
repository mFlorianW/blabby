// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

/**
 * Converts the response of GetMute call into the received Mute.
 */
class BLABBYUPNPAV_EXPORT GetMuteResponse
{
public:
    /**
     * Creates an instance of the GetMuteResponse.
     *
     * @param xmlResponse The XML response of a GetMute call.
     * @param scpd The service control point definition of the RenderingControl service
     * @param action The GetMute action to correctly extract the variables etc. from the XML response.
     */
    GetMuteResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the Mute of the call.
     * @return True when the device reported that the channel is muted.
     */
    bool mute() const noexcept;

private:
    bool mMute = false;
};

} // namespace UPnPAV
