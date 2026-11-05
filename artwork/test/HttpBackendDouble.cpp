// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpBackendDouble.hpp"
#include <QTimer>

namespace Artwork
{

HttpBackendDouble::HttpBackendDouble() = default;
HttpBackendDouble::~HttpBackendDouble() = default;

std::unique_ptr<HttpReply> HttpBackendDouble::get(QNetworkRequest const& request)
{
    mRequests.append(request);
    auto const url = QUrl::fromPercentEncoding(request.url().toEncoded());
    auto response = Response{.urlPart = {}, .status = 404, .body = {}};
    for (auto const& scripted : std::as_const(mResponses)) {
        if (url.contains(scripted.urlPart)) {
            response = scripted;
            break;
        }
    }

    auto reply = std::make_unique<HttpReply>();
    QTimer::singleShot(0, reply.get(), [reply = reply.get(), response] {
        reply->finish(response.status, response.body);
    });
    return reply;
}

void HttpBackendDouble::setResponse(QString const& urlPart, int status, QByteArray const& body)
{
    mResponses.append(Response{.urlPart = urlPart, .status = status, .body = body});
}

QList<QNetworkRequest> const& HttpBackendDouble::requests() const noexcept
{
    return mRequests;
}

QStringList HttpBackendDouble::requestedUrls() const
{
    auto urls = QStringList{};
    for (auto const& request : mRequests) {
        urls.append(QUrl::fromPercentEncoding(request.url().toEncoded()));
    }
    return urls;
}

} // namespace Artwork
