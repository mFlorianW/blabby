// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RequestReader.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <QMutexLocker>
#include <QThread>

namespace Http
{

namespace
{

Request::Method convertMethod(QByteArray const& rawMethod);

} // namespace

RequestReader::RequestReader(qintptr socket)
    : QObject{}
    , mSocket{socket}
{
}

RequestReader::~RequestReader() = default;

ServerRequest RequestReader::serverRequest() const noexcept
{
    auto locker = QMutexLocker{&mMutex};
    return mServerRequest;
}

void RequestReader::readRequest()
{
    auto socket = QTcpSocket{};
    if (not socket.setSocketDescriptor(mSocket)) {
        qCCritical(httpServer) << "Failed to set the Socket";
    }

    llhttp_settings_t settings;
    llhttp_settings_init(&settings);

    settings.on_method = onMethod;

    llhttp_t parser;
    llhttp_init(&parser, HTTP_REQUEST, &settings);
    parser.data = this;

    if (socket.bytesAvailable() == 0) {
        socket.waitForReadyRead();
    }

    while (true) {
        auto data = socket.readAll();
        llhttp_errno error = llhttp_execute(&parser, data.data(), data.size());
        if (error == HPE_OK) {
            break;
        } else if (error == HPE_PAUSED) {
            socket.waitForReadyRead();
        } else {
            qCCritical(httpServer) << "Failed to read request. Error:" << llhttp_errno_name(error);
            break;
        }
        break;
    }

    llhttp_finish(&parser);
    Q_EMIT requestRead();
}

int RequestReader::onMethod(llhttp_t* parser, char const* at, std::size_t length) noexcept
{
    auto* reader = static_cast<RequestReader*>(parser->data);
    auto rawMethod = QByteArray{at, static_cast<qsizetype>(length)};
    {
        auto locker = QMutexLocker{&reader->mMutex};
        reader->mServerRequest.d->mMethod = convertMethod(rawMethod);
    }
    return 0;
}

namespace
{

Request::Method convertMethod(QByteArray const& rawMethod)
{
    auto method = Request::Method::Unknown;
    auto const upperMthd = rawMethod.toUpper();
    if (upperMthd == "DELETE") {
        method = Request::Method::Delete;
    } else if (upperMthd == "GET") {
        method = Request::Method::Get;
    } else if (upperMthd == "HEAD") {
        method = Request::Method::Head;
    } else if (upperMthd == "POST") {
        method = Request::Method::Post;
    } else if (upperMthd == "PUT") {
        method = Request::Method::Put;
    } else if (upperMthd == "CONNECT") {
        method = Request::Method::Connect;
    } else if (upperMthd == "OPTIONS") {
        method = Request::Method::Options;
    } else if (upperMthd == "TRACE") {
        method = Request::Method::Trace;
    } else if (upperMthd == "COPY") {
        method = Request::Method::Copy;
    } else if (upperMthd == "LOCK") {
        method = Request::Method::Lock;
    } else if (upperMthd == "MKCOL") {
        method = Request::Method::MkCol;
    } else if (upperMthd == "MOVE") {
        method = Request::Method::Move;
    } else if (upperMthd == "PROPFIND") {
        method = Request::Method::PropFind;
    } else if (upperMthd == "PROPPATCH") {
        method = Request::Method::PropPatch;
    } else if (upperMthd == "SEARCH") {
        method = Request::Method::Search;
    } else if (upperMthd == "UNLOCK") {
        method = Request::Method::Unlock;
    } else if (upperMthd == "BIND") {
        method = Request::Method::Bind;
    } else if (upperMthd == "REBIND") {
        method = Request::Method::Rebind;
    } else if (upperMthd == "UNBIND") {
        method = Request::Method::Unbind;
    } else if (upperMthd == "ACL") {
        method = Request::Method::Acl;
    } else if (upperMthd == "REPORT") {
        method = Request::Method::Report;
    } else if (upperMthd == "MKACTIVITY") {
        method = Request::Method::MkActivity;
    } else if (upperMthd == "CHECKOUT") {
        method = Request::Method::Checkout;
    } else if (upperMthd == "MERGE") {
        method = Request::Method::Merge;
    } else if (upperMthd == "M-SEARCH") {
        method = Request::Method::MSearch;
    } else if (upperMthd == "NOTIFY") {
        method = Request::Method::Notify;
    } else if (upperMthd == "SUBSCRIBE") {
        method = Request::Method::Subscribe;
    } else if (upperMthd == "UNSUBSCRIBE") {
        method = Request::Method::Unsubscribe;
    } else if (upperMthd == "PATCH") {
        method = Request::Method::Patch;
    } else if (upperMthd == "PURGE") {
        method = Request::Method::Purge;
    } else if (upperMthd == "MKCALENDAR") {
        method = Request::Method::MkCalendar;
    } else if (upperMthd == "LINK") {
        method = Request::Method::Link;
    } else if (upperMthd == "UNLINK") {
        method = Request::Method::Unlink;
    } else if (upperMthd == "SOURCE") {
        method = Request::Method::Source;
    } else if (upperMthd == "PRI") {
        method = Request::Method::Pri;
    } else if (upperMthd == "DESCRIBE") {
        method = Request::Method::Describe;
    } else if (upperMthd == "ANNOUNCE") {
        method = Request::Method::Announce;
    } else if (upperMthd == "SETUP") {
        method = Request::Method::Setup;
    } else if (upperMthd == "PLAY") {
        method = Request::Method::Play;
    } else if (upperMthd == "PAUSE") {
        method = Request::Method::Pause;
    } else if (upperMthd == "TEARDOWN") {
        method = Request::Method::Teardown;
    } else if (upperMthd == "GET_PARAMETER") {
        method = Request::Method::GetParameter;
    } else if (upperMthd == "SET_PARAMETER") {
        method = Request::Method::SetParameter;
    } else if (upperMthd == "REDIRECT") {
        method = Request::Method::Redirect;
    } else if (upperMthd == "RECORD") {
        method = Request::Method::Record;
    } else if (upperMthd == "FLUSH") {
        method = Request::Method::Flush;
    }

    return method;
}

} // namespace

} // namespace Http
