// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServerRequest.hpp"

namespace Http
{

ServerRequest::ServerRequest()
    : d{new ServerRequestData{}}
{
}

Request::Method ServerRequest::method() const noexcept
{
    return d->mMethod;
}

Headers const& ServerRequest::headers() const noexcept
{
    return d->mHeaders;
}

QUrl const& ServerRequest::url() const noexcept
{
    return d->mUrl;
}

QByteArray const& ServerRequest::body() const noexcept
{
    return d->mBody;
}

} // namespace Http
