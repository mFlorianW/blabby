// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbyhttp_export.h"
#include <QMetaEnum>
#include <QSharedData>

namespace Http
{
struct ServerResponseData;

namespace Response
{
Q_NAMESPACE_EXPORT(BLABBYHTTP_EXPORT)

/**
 * The status code of the @ref Http::ServerResponse
 */
enum class StatusCode
{
    // Informational
    Continue = 100,
    SwitchingProtocols = 101,
    Processing = 102,

    // Successful
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultiStatus = 207,
    AlreadyReported = 208,
    ImUsed = 226,

    // Redirection
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    SeeOther = 303,
    NotModified = 304,
    UseProxy = 305,
    SwitchProxy = 306,
    TemporaryRedirect = 307,
    PermanentRedirect = 308,

    // Client Errors
    BadRequest = 400,
    Unauthorized = 401,
    PaymentRequired = 402,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    NotAcceptable = 406,
    ProxyAuthenticationRequired = 407,
    RequestTimeout = 408,
    Conflict = 409,
    Gone = 410,
    LengthRequired = 411,
    PreconditionFailed = 412,
    PayloadTooLarge = 413,
    UriTooLong = 414,
    UnsupportedMediaType = 415,
    RangeNotSatisfiable = 416,
    ExpectationFailed = 417,
    MisdirectedRequest = 421,
    UnprocessableEntity = 422,
    Locked = 423,
    FailedDependency = 424,
    UpgradeRequired = 426,
    PreconditionRequired = 428,
    TooManyRequests = 429,
    RequestHeaderFieldsTooLarge = 431,
    UnavailableForLegalReasons = 451,

    // Server Errors
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504,
    HttpVersionNotSupported = 505,
    VariantAlsoNegotiates = 506,
    InsufficientStorage = 507,
    LoopDetected = 508,
    NotExtended = 510,
    NetworkAuthenticationRequired = 511
};
Q_ENUM_NS(StatusCode)

} // namespace Response

/**
 * The @ref Http::ServerResponse is the deserialized HTTP response to client to answer a request.
 */
class BLABBYHTTP_EXPORT ServerResponse
{
public:
    ServerResponse();

    Response::StatusCode statusCode() const noexcept;
    void setStatusCode(Response::StatusCode statusCode) noexcept;

private:
    QSharedDataPointer<ServerResponseData> d;
};

struct ServerResponseData : public QSharedData
{
    Response::StatusCode mStatusCode = Response::StatusCode::Ok;
};

} // namespace Http
