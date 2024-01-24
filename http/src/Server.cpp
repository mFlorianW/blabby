// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Server.hpp"
#include "private/ServerPrivate.hpp"

namespace Http
{

Server::Server()
    : QTcpServer{}
    , d{std::make_unique<ServerPrivate>()}
{
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server() = default;

void Server::onNewConnection() noexcept
{
    while (hasPendingConnections()) {
        auto con = std::make_unique<ClientConnection>(nextPendingConnection());
        connect(con.get(), &ClientConnection::requestReceived, this, &Server::onRequestReceived);
        connect(con.get(), &ClientConnection::responseSent, this, &Server::onResponseSent);
        con->readRequest();
        d->mClients.insert({con.get(), std::move(con)});
    }
}

void Server::onRequestReceived(ServerRequest const& request, ClientConnection* connection) noexcept
{
    auto resp = ServerResponse{};
    if (handleRequest(request, resp)) {
        connection->sendResponse(resp);
    }
}

void Server::onResponseSent(Http::ClientConnection* connection)
{
    if (d->mClients.contains(connection)) {
        d->mClients[connection]->close();
        d->mClients.erase(connection);
    }
}

} // namespace Http
