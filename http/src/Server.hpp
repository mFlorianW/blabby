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

protected:
    /**
     * Pure virtual function for incoming requests.
     * Concrete server implementations must implement the function and shall handle the request that are interessed for.
     * The function shall return true for a handled request and false in any other case.
     * @param request The request that the server receives.
     * @param response The response which is send from server back to the client. E.g. HTTP 200/OK
     */
    virtual bool handleRequest(ServerRequest const& request, ServerResponse& response) = 0;

    /**
     * This slot is called for every new connection.
     */
    void onNewConnection() noexcept;

private Q_SLOTS:
    void onRequestReceived(Http::ServerRequest const& request, Http::ClientConnection* connection) noexcept;
    void onResponseSent(Http::ClientConnection* connection);

private:
    std::unique_ptr<ServerPrivate> d;
};

} // namespace Http
