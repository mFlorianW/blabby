// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbyhttp_export.h"
#include <QMetaEnum>
#include <QSharedData>
#include <QUrl>

namespace Http
{

namespace Request
{
Q_NAMESPACE_EXPORT(BLABBYHTTP_EXPORT)

/**
 * Method of the @ref Http::ServerRequest
 */
enum class BLABBYHTTP_EXPORT Method
{
    Unknown,
    Delete,
    Get,
    Head,
    Post,
    Put,
    Connect,
    Options,
    Trace,
    Copy,
    Lock,
    MkCol,
    Move,
    PropFind,
    PropPatch,
    Search,
    Unlock,
    Bind,
    Rebind,
    Unbind,
    Acl,
    Report,
    MkActivity,
    Checkout,
    Merge,
    MSearch,
    Notify,
    Subscribe,
    Unsubscribe,
    Patch,
    Purge,
    MkCalendar,
    Link,
    Unlink,
    Source,
    Pri,
    Describe,
    Announce,
    Setup,
    Play,
    Pause,
    Teardown,
    GetParameter,
    SetParameter,
    Redirect,
    Record,
    Flush,
};
Q_ENUM_NS(Method)

} // namespace Request

using Headers = QHash<QByteArray, QByteArray>;
class RequestDeserializer;
struct ServerRequestData;
/**
 * The @ref Http::ServerRequest is a deserialized HTTP request, which was send from a client to server.
 */
class BLABBYHTTP_EXPORT ServerRequest
{
public:
    /**
     * Creates an instance of a ServerRequest
     */
    ServerRequest();

    /**
     * Gives the HTTP method of the request
     * @return The HTTP method
     */
    Request::Method method() const noexcept;

    /**
     * Gives all HTTP headers of the client request.
     * All HTTP headers from the client request.
     * @return A hash map with all headers of the request.
     */
    Headers const& headers() const noexcept;

    /**
     * Gives the requested URL
     * @return The requested URL.
     */
    QUrl const& url() const noexcept;

    /**
     * Gives the body of the request
     * @return A QByteArray that contains the body of the request.
     */
    QByteArray const& body() const noexcept;

private:
    friend Http::RequestDeserializer;
    QExplicitlySharedDataPointer<ServerRequestData> d;
};

/**
 * Shared data of a ServerRequest instance
 */
struct ServerRequestData : public QSharedData
{
    Request::Method mMethod = Request::Method::Unknown;
    Headers mHeaders;
    QUrl mUrl;
    QByteArray mBody;
};

} // namespace Http
