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
    InstanceVariables const& instanceVariables() noexcept;

private:
    QString mLastChange;
    InstanceVariables mInstanceVariables;
};

} // namespace UPnPAV
