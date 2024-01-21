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

} // namespace Http
