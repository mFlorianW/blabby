// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServerResponse.hpp"

namespace Http
{

ServerResponse::ServerResponse()
    : d{new ServerResponseData}
{
}

Response::StatusCode ServerResponse::statusCode() const noexcept
{
    return d->mStatusCode;
}

void ServerResponse::setStatusCode(Response::StatusCode statusCode) noexcept
{
    d->mStatusCode = statusCode;
}

Headers const& ServerResponse::headers() const noexcept
{
    return d->mHeaders;
}

void ServerResponse::setHeader(QByteArray const& header, QByteArray const& value) noexcept
{
    d->mHeaders.insert(header, value);
}

QByteArray const& ServerResponse::body() const noexcept
{
    return d->mBody;
}

void ServerResponse::setBody(QByteArray const& body) noexcept
{
    d->mBody = body;
}

} // namespace Http
