// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServerRequest.hpp"
#include "blabbyhttp_export.h"
#include <QMutex>
#include <QObject>
#include <QTcpSocket>
#include <llhttp.h>

namespace Http
{

/**
 * The @ref Http::RequestReader is worker object for asynchronous receiving of HTTP requests.
 * An instance is automatically created when the @ref Http::ClientConnection starts to read the request.
 */
class BLABBYHTTP_EXPORT RequestReader final : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates and instance of the RequestReader
     * @param connection The TCP client connection for reading.
     */
    RequestReader(qintptr connection);

    /**
     * Default destructor
     */
    ~RequestReader() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(RequestReader)

    /**
     * Gives the read HTTP request.
     * The request is only valid when the signal @ref Http::RequestReader::requestRead is emitted.
     * @param The complete parsed @ref Http::RequestReader::ServerRequest.
     */
    ServerRequest serverRequest() const noexcept;

public Q_SLOTS:
    /**
     * Starts the request reading
     * The signal @ref Http::RequestReader::requestRead is emitted on complete parsed packages.
     */
    void readRequest();

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Http::RequestReader has read the complete request.
     */
    void requestRead();

private:
    static int onMethod(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onHeader(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onHeaderValue(llhttp_t* parser, char const* at, std::size_t length) noexcept;

private:
    mutable QMutex mMutex;
    ServerRequest mServerRequest;
    qintptr mSocket;
    Headers::iterator mLastHeader;
};

} // namespace Http
