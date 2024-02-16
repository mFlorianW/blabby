// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServerRequest.hpp"
#include "ServerResponse.hpp"
#include "blabbyhttp_export.h"
#include <QHostAddress>
#include <QTcpServer>
#include <QtGlobal>

namespace Http
{
struct ServerPrivate;
class ClientConnection;

/**
 * The @ref Http::Server can receive HTTP requests from network devices.
 * @note
 * The @ref Http::Server should only be used in LAN and known environments.
 */
class BLABBYHTTP_EXPORT Server : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * Callback definition for routes in the server.
     */
    using RouteCallback = std::function<bool(ServerRequest const&, ServerResponse&)>;

    /**
     * Default constructor
     */
    Server();

    /**
     * Default destructor
     */
    ~Server() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(Server)

    /**
     * Adds a route the server.
     * The the passed @ref Http::ServerRequest::RouteCallback is called when a request with route is received.
     * @param route The route that shall be registered.
     * @param routeCallback The call that shall be called when a Request for that route is received.
     *                      A callback should return the true when the @ref Http::ServerRequest is completly handled,
     *                      returning false from this function will forward the @ref Http::ServerRequest to the
     *                      @ref Http::ServerRequest::handleRequest function.
     * @note
     * Calling the route function multiple time with the same route will overwrite previous added route.
     */
    void addRoute(QByteArray const& route, RouteCallback const& routeCallback) noexcept;

    /**
     * Removes a route from the server.
     * If the route was not present nothing happens.
     * @param route The route which shall be removed.
     */
    void removeRoute(QByteArray const& route) noexcept;

protected:
    /**
     * Pure virtual function for incoming requests.
     * Concrete server implementations must implement the function and shall handle the request that are interessed for.
     * The default implementation does nothing.
     * @param request The request that the server receives.
     * @param response The response which is send from server back to the client. E.g. HTTP 200/OK
     */
    virtual void handleRequest(ServerRequest const& request, ServerResponse& response);

    /**
     * Virtual function for failed requests.
     * Concrete server implementations can override this function to handle failed requests.
     * The server will force the HTTP response code to "BadRequest".
     * @param request The request that the server receives.
     * @param response The response which is send from the server back to the client.
     */
    virtual void handleFailedRequest(ServerRequest const& request, ServerResponse& response) noexcept;

    /**
     * This slot is called for every new connection.
     */
    void onNewConnection() noexcept;

private Q_SLOTS:
    void onRequestReceived(Http::ServerRequest const& request, Http::ClientConnection* connection) noexcept;
    void onRequestReceivedFailed(Http::ServerRequest const& request, Http::ClientConnection* connection) noexcept;
    void onResponseSent(Http::ClientConnection* connection);

private:
    std::unique_ptr<ServerPrivate> d;
    std::unordered_map<QByteArray, RouteCallback> mRoutes;
};

} // namespace Http
