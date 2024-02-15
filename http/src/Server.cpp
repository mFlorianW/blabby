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

void Server::addRoute(QByteArray const& route, RouteCallback const& routeCallback) noexcept
{
    mRoutes.insert({route, routeCallback});
}

void Server::removeRoute(QByteArray const& route) noexcept
{
    mRoutes.erase(route);
}

void Server::handleRequest(ServerRequest const& request, ServerResponse& resp)
{
    Q_UNUSED(request)
    Q_UNUSED(resp)
}

void Server::handleFailedRequest(ServerRequest const& request, ServerResponse& response) noexcept
{
    Q_UNUSED(request)
    Q_UNUSED(response)
}

void Server::onNewConnection() noexcept
{
    while (hasPendingConnections()) {
        auto con = std::make_unique<ClientConnection>(nextPendingConnection());
        connect(con.get(), &ClientConnection::requestReceived, this, &Server::onRequestReceived);
        connect(con.get(), &ClientConnection::requestReceivedFailed, this, &Server::onRequestReceivedFailed);
        connect(con.get(), &ClientConnection::responseSent, this, &Server::onResponseSent);
        con->readRequest();
        d->mClients.insert({con.get(), std::move(con)});
    }
}

void Server::onRequestReceived(ServerRequest const& request, ClientConnection* connection) noexcept
{
    auto resp = ServerResponse{};
    resp.setStatusCode(Response::StatusCode::NotFound);
    auto handled = false;
    auto url = request.url().toString().toUtf8();

    if (mRoutes.find(url) != mRoutes.end()) {
        handled = mRoutes.at(url)(request, resp);
    }

    if (not handled) {
        handleRequest(request, resp);
    }

    connection->sendResponse(resp);
}

void Server::onRequestReceivedFailed(Http::ServerRequest const& request, Http::ClientConnection* connection) noexcept
{
    auto resp = ServerResponse{};
    handleFailedRequest(request, resp);
    resp.setStatusCode(Response::StatusCode::BadRequest);
    connection->sendResponse(resp);
}

void Server::onResponseSent(Http::ClientConnection* connection)
{
    if (d->mClients.contains(connection)) {
        d->mClients[connection]->close();
        d->mClients.erase(connection);
    }
}

} // namespace Http
