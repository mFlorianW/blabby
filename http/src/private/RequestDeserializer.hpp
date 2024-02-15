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
class BLABBYHTTP_EXPORT RequestDeserializer final : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates and instance of the RequestReader
     * @param rawRequest The raw request for parsing.
     */
    RequestDeserializer(QByteArray rawRequest);

    /**
     * Default destructor
     */
    ~RequestDeserializer() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(RequestDeserializer)

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

    /**
     * This signal is emitted when it's failed to read the request
     */
    void requestReadFailed();

private:
    static int onMethod(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onHeader(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onHeaderValue(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onUrl(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onBody(llhttp_t* parser, char const* at, std::size_t length) noexcept;
    static int onMessageComplete(llhttp_t* parser) noexcept;

private:
    mutable QMutex mMutex;
    ServerRequest mServerRequest;
    QByteArray mRequest;
    Headers::iterator mLastHeader;
    bool mMessageComplete = false;
};

} // namespace Http
