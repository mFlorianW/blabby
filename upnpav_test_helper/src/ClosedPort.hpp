// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QTcpServer>

namespace UPnPAV
{

/**
 * Gives a local port that nobody listens on, a request to it fails like a request to an unreachable device.
 * @return The local port.
 */
inline quint16 closedPort()
{
    auto server = QTcpServer{};
    server.listen(QHostAddress::LocalHost);
    return server.serverPort();
}

} // namespace UPnPAV
