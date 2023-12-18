// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include "MediaServerObject.hpp"
#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

/**
 * Helper object for easier building the @ref UPnPAV::MediaObject
 */
class BLABBYUPNPAV_EXPORT MediaServerObjectBuilder
{
public:
    /**
     * Sets the ID of the MediaObject
     * @param id The ID of the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withId(QString const &id) noexcept;

    /**
     * Sets the parent ID of the MediaObject
     * @param parentId The parent ID of the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withParentId(QString const &parentId) noexcept;

    /**
     * Sets the titleof the MediaObject
     * @param id The title of the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withTitle(QString const &title) noexcept;

    /**
     * Sets the type class of the MediaObject
     * @param id The type class of the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withTypeClass(QString const &typeClass) noexcept;

    /**
     * Sets the Id of the MediaObject
     * @param id The play URL of the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withPlayUrl(QString const &playUrl) noexcept;

    /**
     * Set the supported protocols in the build @ref UPnPAV::MediaServerObject
     * @param supportedProtocols The list of supported protocols for the @ref UPnPAV::MediaServerObject
     * @return Reference to the @ref UPnPAV::MediaServerObjectBuilder for chaining
     */
    MediaServerObjectBuilder &withSupportedProtocols(QVector<Protocol> const &supportedProtocols) noexcept;

    /**
     * Checks if the required parameters are correctly set. This can be helpfull
     * before calling build.
     */
    bool isValid() const noexcept;

    /**
     * Gives the consturcted MediaServerObject.
     * @return The consturcted MediaServerObject.
     */
    MediaServerObject build();

private:
    MediaServerObject mObj;
};

} // namespace UPnPAV
