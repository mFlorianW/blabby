// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpBackend.hpp"
#include <QList>

namespace Artwork
{

/**
 * A @ref HttpBackend that answers with scripted responses and remembers the requests it got.
 * The replies finish asynchronously, an unscripted URL is answered with 404.
 */
class HttpBackendDouble final : public HttpBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HttpBackendDouble)
public:
    HttpBackendDouble();
    ~HttpBackendDouble() override;

    std::unique_ptr<HttpReply> get(QNetworkRequest const& request) override;

    /**
     * Scripts the response to the requests whose percent decoded URL contains the given part.
     * The first scripted response that matches is used.
     * @param urlPart The part of the URL.
     * @param status The HTTP status, 0 for a request that fails without a response.
     * @param body The body of the response.
     */
    void setResponse(QString const& urlPart, int status, QByteArray const& body = {});

    /**
     * Gives all requests in the order they were sent.
     */
    QList<QNetworkRequest> const& requests() const noexcept;

    /**
     * Gives the percent decoded URLs of all requests in the order they were sent.
     */
    QStringList requestedUrls() const;

private:
    struct Response
    {
        QString urlPart;
        int status = 0;
        QByteArray body;
    };

    QList<Response> mResponses;
    QList<QNetworkRequest> mRequests;
};

} // namespace Artwork
