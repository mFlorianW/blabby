// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServerRequest.hpp"
#include "ServerResponse.hpp"
#include "blabbyhttp_export.h"
#include "private/RequestReader.hpp"
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <memory>

namespace Http
{

/**
 * The @ref Http::ClientConnection is created for every client which sends a request to the server
 */
class BLABBYHTTP_EXPORT ClientConnection final : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a ClientConnection instance
     * @param connection The TCP connection for reading and sending.
     */
    ClientConnection(qintptr socketDesc);

    /**
     * Destructor cleansup reader and sender threads.
     */
    ~ClientConnection() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(ClientConnection)

    /**
     * Starts the reading and parsing of the HTTP request.
     * The signal requestReceived is emitted when the request is completly received and parsed.
     */
    void readRequest() noexcept;

Q_SIGNALS:
    /**
     * The signal is emitted when the whole complete request is received.
     * @param request The parsed server request from the client.
     * @param connection The client connection that has finished reading the request.
     */
    void requestReceived(Http::ServerRequest const& request, Http::ClientConnection* connection);

private:
    void stopReadThread();

private:
    qintptr mSocketDesc;
    ServerRequest mRequest;
    QThread mReadThread;
    std::unique_ptr<RequestReader> mReader;
};

} // namespace Http
