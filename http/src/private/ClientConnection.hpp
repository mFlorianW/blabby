// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ResponseSerializer.hpp"
#include "ServerRequest.hpp"
#include "ServerResponse.hpp"
#include "blabbyhttp_export.h"
#include "private/RequestDeserializer.hpp"
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
    ClientConnection(QTcpSocket* socketDesc);

    /**
     * Destructor cleansup reader and sender threads.
     */
    ~ClientConnection() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(ClientConnection)

    /**
     * Closes the @ref HTTP::ClientConnection.
     * No further data are received or emitted.
     */
    void close();

    /**
     * Starts the reading and parsing of the HTTP request.
     * The signal requestReceived is emitted when the request is completly received and parsed.
     */
    void readRequest() noexcept;

    /**
     * Sends the passed @ref Http::ServerResponse to the client
     * @param serverResponse The Response which shall be send to the client.
     *
     * @note
     * The signal @ref Http::ClientConnection::responseSent is emitted when the response is sended to the client.
     */
    void sendResponse(ServerResponse const& serverResponse) noexcept;

Q_SIGNALS:
    /**
     * The signal is emitted when the whole complete request is received.
     * @param request The parsed server request from the client.
     * @param connection The client connection that has finished reading the request.
     */
    void requestReceived(Http::ServerRequest const& request, Http::ClientConnection* connection);

    /**
     * This signal is emitted when a @ref Http::ServerResponse is send to the client.
     * @param connection The connection for that the response was send.
     */
    void responseSent(Http::ClientConnection* connection);

private:
    void stopThread(QThread& thread);

private:
    QTcpSocket* mSocket;
    ServerRequest mRequest;
    QThread mReadThread;
    std::unique_ptr<RequestDeserializer> mReader;
    QThread mWriteThread;
    std::unique_ptr<ResponseSerializer> mWriter;
    qsizetype mResponseSize = qsizetype{0};
};

} // namespace Http
