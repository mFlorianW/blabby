// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QString>

namespace UPnPAV
{

/**
 * Gives an event variable of a LastChange event, e.g. <TransportState val="PLAYING"/>.
 * @param name The name of the variable.
 * @param value The value of the variable, it is escaped.
 * @param channel The channel of the variable, e.g. Master for the Volume, left out when empty.
 */
inline QString lastChangeVariable(QString const& name, QString const& value, QString const& channel = QString{})
{
    if (channel.isEmpty()) {
        return QStringLiteral(R"(<%1 val="%2"/>)").arg(name, value.toHtmlEscaped());
    }
    return QStringLiteral(R"(<%1 channel="%2" val="%3"/>)").arg(name, channel, value.toHtmlEscaped());
}

/**
 * Gives the body of a NOTIFY request with a LastChange event for the instance ID 0.
 * @param variables The event variables of the instance, e.g. made with @ref lastChangeVariable.
 */
inline QString lastChangeNotify(QString const& variables)
{
    auto const event = QStringLiteral(R"(<Event xmlns="urn:schemas-upnp-org:metadata-1-0/AVT/">)"
                                      R"(<InstanceID val="0">%1</InstanceID></Event>)")
                           .arg(variables);
    return QStringLiteral(R"(<e:propertyset xmlns:e="urn:schemas-upnp-org:event-1-0">)"
                          R"(<e:property><LastChange>%1</LastChange></e:property></e:propertyset>)")
        .arg(event.toHtmlEscaped());
}

} // namespace UPnPAV
