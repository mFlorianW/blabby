// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

/**
 * Converts the response of GetVolume call into the received volume.
 */
class BLABBYUPNPAV_EXPORT GetVolumeResponse
{
public:
    /**
     * Creates an instance of the GetVolumeResponse.
     *
     * @param xmlResponse The XML response of a GetVolume call.
     * @param scpd The service control point definition of the RenderingControl service
     * @param action The GetVolume action to correctly extrac the variables etc. from the XML response.
     */
    GetVolumeResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);

    /**
     * Gives the volume of the call.
     * @return The volume reported by the device.
     */
    quint32 volume() const noexcept;

private:
    quint32 mVolume = 0U;
};

} // namespace UPnPAV
