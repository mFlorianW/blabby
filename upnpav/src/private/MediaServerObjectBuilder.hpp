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
     * @return Reference to the builder for chaining
     */
    MediaServerObjectBuilder &setId(QString const &id) noexcept;

    /**
     * Sets the parent ID of the MediaObject
     * @param parentId The parent ID of the @ref UPnPAV::MediaServerObject
     * @return Reference to the builder for chaining
     */
    MediaServerObjectBuilder &setParentId(QString const &parentId) noexcept;

    /**
     * Sets the titleof the MediaObject
     * @param id The title of the @ref UPnPAV::MediaServerObject
     * @return Reference to the builder for chaining
     */
    MediaServerObjectBuilder &setTitle(QString const &title) noexcept;

    /**
     * Sets the type class of the MediaObject
     * @param id The type class of the @ref UPnPAV::MediaServerObject
     * @return Reference to the builder for chaining
     */
    MediaServerObjectBuilder &setTypeClass(QString const &typeClass) noexcept;

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
