// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbyartwork_export.h"
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <memory>

class QNetworkReply;

namespace Artwork
{

/**
 * The @ref Artwork::HttpReply is the answer to a GET request of a @ref Artwork::HttpBackend.
 * It's finished once, either with the HTTP status and the body of the response or with a network error.
 */
class BLABBYARTWORK_EXPORT HttpReply : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HttpReply)
public:
    HttpReply();
    ~HttpReply() override;

    /**
     * Gives the HTTP status of the response, 0 when the request failed without a response, e.g. without internet
     * access.
     */
    int status() const noexcept;

    /**
     * Gives the body of the response, empty without a response.
     */
    QByteArray const& body() const noexcept;

    /**
     * Finishes the reply and emits @ref finished.
     * @param status The HTTP status, 0 when the request failed without a response.
     * @param body The body of the response.
     */
    void finish(int status, QByteArray body);

Q_SIGNALS:
    /**
     * This signal is emitted when the reply is finished.
     */
    void finished();

private:
    int mStatus = 0;
    QByteArray mBody;
};

/**
 * The @ref Artwork::HttpBackend sends the HTTP GET requests of the Artwork lookup.
 * It exists so that the lookup can be tested without the internet.
 */
class BLABBYARTWORK_EXPORT HttpBackend : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HttpBackend)
public:
    ~HttpBackend() override;

    /**
     * Sends a GET request.
     * @param request The URL and the headers of the request.
     * @return The reply, that finishes asynchronously. Destroying the reply aborts the request.
     */
    virtual std::unique_ptr<HttpReply> get(QNetworkRequest const& request) = 0;

protected:
    HttpBackend() = default;
};

/**
 * The @ref Artwork::NetworkHttpBackend sends the requests with Qt's network access.
 */
class BLABBYARTWORK_EXPORT NetworkHttpBackend final : public HttpBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(NetworkHttpBackend)
public:
    NetworkHttpBackend();
    ~NetworkHttpBackend() override;

    std::unique_ptr<HttpReply> get(QNetworkRequest const& request) override;

private:
    QNetworkAccessManager mNetworkAccess;
};

} // namespace Artwork
