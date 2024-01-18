// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Server.hpp"
#include "private/ServerPrivate.hpp"

namespace Http
{

Server::Server()
    : QObject{}
    , d{std::make_unique<ServerPrivate>()}
{
    connect(&d->mTcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server() = default;

bool Server::bind(QHostAddress const& address, quint16 port) noexcept
{
    return d->mTcpServer.listen(address, port);
}

void Server::onNewConnection() noexcept
{
    while (d->mTcpServer.hasPendingConnections()) {
        auto socket = d->mTcpServer.nextPendingConnection();
        auto con = std::make_unique<ClientConnection>(socket);
        connect(con.get(), &ClientConnection::requestReceived, this, &Server::onRequestReceived);
        con->readRequest();
        d->mClients.insert({con.get(), std::move(con)});
    }
}

void Server::onRequestReceived(ServerRequest const& request) noexcept
{
    auto resp = ServerResponse{};
    handleRequest(request, resp);
}

} // namespace Http
