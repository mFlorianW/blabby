// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "private/ClientConnection.hpp"
#include <QTcpServer>
#include <unordered_map>

namespace Http
{

/**
 * Private data members of the server
 */
struct ServerPrivate
{
    QTcpServer mTcpServer;
    std::unordered_map<ClientConnection*, std::unique_ptr<ClientConnection>> mClients;
};

} // namespace Http
