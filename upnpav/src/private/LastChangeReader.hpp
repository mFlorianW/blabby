// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QHash>
#include <QString>

namespace UPnPAV
{

using Variables = QHash<QString, QString>;
using InstanceVariables = QHash<QString, Variables>;

/**
 * Reads all the instance ID elemets with the varialbes.
 * The "LastChange" data is typically recveied from NOTFIY message of some UPnPAV services.
 */
class LastChangeReader
{
public:
    /**
     * Creates a LastChangeReader instance with the raw last change data
     * @param lastChange The raw data that shall be read
     */
    explicit LastChangeReader(QString lastChange) noexcept;

    /**
     * Reads the data and extracts all the instance ids with the variables.
     * @return True everything is read, false something went wrong.
     */
    bool read() noexcept;

    /**
     * Gives the instance ids with the variables.
     * The return varialbes is only valid after @UPnPAV::LastChangeReader::read was succesful called.
     * @return The extracted LastChange data
     */
    InstanceVariables const& instanceVariables() const noexcept;

    /**
     * Gives the instance ids with the corresponding volume variables.
     * The volume variables are handled differentialy because the volume keyword occurs multiple times in the last
     * change event because the volume is reported for every channel, e.g. Master, Left/Right front etc.
     *
     * The volume variables are only valid for RenderingControl services for all other services this variable will be
     * empty. The channel attribute is the key of the returned hash map and the value contains the volume.
     * @return A hash map containing the volumes for every instance ID.
     */
    InstanceVariables const& instanceVolumeVariables() const noexcept;

private:
    QString mLastChange;
    InstanceVariables mInstanceVariables;
    InstanceVariables mInstanceVolumeVariables;
};

} // namespace UPnPAV
