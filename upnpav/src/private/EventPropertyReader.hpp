// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QHash>
#include <QString>

namespace UPnPAV
{

/**
 * Reads all the changed properites of UPnP NOTIFY message
 */
class EventPropertyReader final
{
public:
    /**
     * Creates an EventPropertyReader instance
     * @param The NOTIFY message content that shall be read.
     */
    explicit EventPropertyReader(QString responseBody) noexcept;

    /**
     * Reads the reponse body and caches the read properties
     * @return True everything read, false something went wrong.
     */
    bool read();

    /**
     * Gives the values for the property.
     * @return The value for the property, if the value is not found an nullopt;
     */
    std::optional<QString> property(QString const& propertyName);

private:
    QString mResponseBody;
    QHash<QString, QString> mProperties;
};

} // namespace UPnPAV
