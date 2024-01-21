// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServerShould.hpp"
#include <QNetworkReply>
#include <QTest>

namespace Http
{

constexpr quint32 timeout = 30000;

bool TestServer::handleRequestCalled() const noexcept
{
    return mHandleRequestCalled;
}

bool TestServer::handleRequest(ServerRequest const& request, ServerResponse& response)
{
    Q_UNUSED(response);

    mLastRequest = request;
    mHandleRequestCalled = true;

    return true;
}

ServerRequest TestServer::serverRequest() const noexcept
{
    return mLastRequest;
}

void TestServer::resetStates() noexcept
{
    mHandleRequestCalled = false;
    mLastRequest = ServerRequest{};
}

ServerShould::~ServerShould() = default;

void ServerShould::initTestCase()
{
    mHtppServer = std::make_unique<TestServer>();
    QCOMPARE(mHtppServer->listen(QHostAddress::LocalHost, 27016), true);
}

void ServerShould::init()
{
    mHtppServer->resetStates();
}

void ServerShould::give_a_request_with_the_request_method_data()
{
    QTest::addColumn<QByteArray>("method");
    QTest::addColumn<Request::Method>("methodEnum");

    // clang-format off
    QTest::newRow("GET") << QByteArray{"GET"} << Request::Method::Get;
    QTest::newRow("DELETE") << QByteArray{"DELETE"} << Request::Method::Delete;
    QTest::newRow("HEAD") << QByteArray{"HEAD"} << Request::Method::Head;
    QTest::newRow("POST") << QByteArray{"POST"} << Request::Method::Post;
    QTest::newRow("PUT") << QByteArray{"PUT"} << Request::Method::Put;
    QTest::newRow("CONNECT") << QByteArray{"CONNECT"} << Request::Method::Connect;
    QTest::newRow("OPTIONS") << QByteArray{"OPTIONS"} << Request::Method::Options;
    QTest::newRow("TRACE") << QByteArray{"TRACE"} << Request::Method::Trace;
    QTest::newRow("COPY") << QByteArray{"COPY"} << Request::Method::Copy;
    QTest::newRow("LOCK") << QByteArray{"LOCK"} << Request::Method::Lock;
    QTest::newRow("MKCOL") << QByteArray{"MKCOL"} << Request::Method::MkCol;
    QTest::newRow("MOVE") << QByteArray{"MOVE"} << Request::Method::Move;
    QTest::newRow("PROPFIND") << QByteArray{"PROPFIND"} << Request::Method::PropFind;
    QTest::newRow("PROPPATCH") << QByteArray{"PROPPATCH"} << Request::Method::PropPatch;
    QTest::newRow("SEARCH") << QByteArray{"SEARCH"} << Request::Method::Search;
    QTest::newRow("UNLOCK") << QByteArray{"UNLOCK"} << Request::Method::Unlock;
    QTest::newRow("BIND") << QByteArray{"BIND"} << Request::Method::Bind;
    QTest::newRow("REBIND") << QByteArray{"REBIND"} << Request::Method::Rebind;
    QTest::newRow("UNBIND") << QByteArray{"UNBIND"} << Request::Method::Unbind;
    QTest::newRow("ACL") << QByteArray{"ACL"} << Request::Method::Acl;
    QTest::newRow("REPORT") << QByteArray{"REPORT"} << Request::Method::Report;
    QTest::newRow("MKACTIVITY") << QByteArray{"MKACTIVITY"} << Request::Method::MkActivity;
    QTest::newRow("CHECKOUT") << QByteArray{"CHECKOUT"} << Request::Method::Checkout;
    QTest::newRow("MERGE") << QByteArray{"MERGE"} << Request::Method::Merge;
    QTest::newRow("M-SEARCH") << QByteArray{"M-SEARCH"} << Request::Method::MSearch;
    QTest::newRow("NOTIFY") << QByteArray{"NOTIFY"} << Request::Method::Notify;
    QTest::newRow("SUBSCRIBE") << QByteArray{"SUBSCRIBE"} << Request::Method::Subscribe;
    QTest::newRow("UNSUBSCRIBE") << QByteArray{"UNSUBSCRIBE"} << Request::Method::Unsubscribe;
    QTest::newRow("PATCH") << QByteArray{"PATCH"} << Request::Method::Patch;
    QTest::newRow("PURGE") << QByteArray{"PURGE"} << Request::Method::Purge;
    QTest::newRow("MKCALENDAR") << QByteArray{"MKCALENDAR"} << Request::Method::MkCalendar;
    QTest::newRow("LINK") << QByteArray{"LINK"} << Request::Method::Link;
    QTest::newRow("UNLINK") << QByteArray{"UNLINK"} << Request::Method::Unlink;
    QTest::newRow("SOURCE") << QByteArray{"SOURCE"} << Request::Method::Source;
    QTest::newRow("PRI") << QByteArray{"PRI"} << Request::Method::Pri;
    QTest::newRow("DESCRIBE") << QByteArray{"DESCRIBE"} << Request::Method::Describe;
    QTest::newRow("ANNOUNCE") << QByteArray{"ANNOUNCE"} << Request::Method::Announce;
    QTest::newRow("SETUP") << QByteArray{"SETUP"} << Request::Method::Setup;
    QTest::newRow("PLAY") << QByteArray{"PLAY"} << Request::Method::Play;
    QTest::newRow("PAUSE") << QByteArray{"PAUSE"} << Request::Method::Pause;
    QTest::newRow("TEARDOWN") << QByteArray{"TEARDOWN"} << Request::Method::Teardown;
    QTest::newRow("GET_PARAMETER") << QByteArray{"GET_PARAMETER"} << Request::Method::GetParameter;
    QTest::newRow("SET_PARAMETER") << QByteArray{"SET_PARAMETER"} << Request::Method::SetParameter;
    QTest::newRow("REDIRECT") << QByteArray{"REDIRECT"} << Request::Method::Redirect;
    QTest::newRow("RECORD") << QByteArray{"RECORD"} << Request::Method::Record;
    QTest::newRow("FLUSH") << QByteArray{"FLUSH"} << Request::Method::Flush;
    // clang-format on
}

void ServerShould::give_a_request_with_the_request_method()
{
    QFETCH(QByteArray, method);
    QFETCH(Request::Method, methodEnum);
    auto req = QNetworkRequest{};
    req.setUrl(QUrl{"http://127.0.0.1:27016"});
    auto reply = mHttpClient.sendCustomRequest(req, method);

    QTRY_COMPARE_WITH_TIMEOUT(mHtppServer->handleRequestCalled(), true, timeout);
    QCOMPARE(mHtppServer->serverRequest().method(), methodEnum);
    reply->deleteLater();
}

void ServerShould::give_a_request_with_the_headers()
{
    auto req = QNetworkRequest{};
    req.setUrl(QUrl{"http://127.0.0.1:27016"});
    req.setRawHeader(QByteArray{"Test"}, QByteArray{"TestValue"});
    auto reply = mHttpClient.get(req);

    QTRY_COMPARE_WITH_TIMEOUT(mHtppServer->handleRequestCalled(), true, timeout);
    QCOMPARE(mHtppServer->serverRequest().headers().contains("Test"), true);
    QCOMPARE(mHtppServer->serverRequest().headers().value("Test"), QByteArray{"TestValue"});
    reply->deleteLater();
}

void ServerShould::give_a_request_with_the_request_url()
{
    auto req = QNetworkRequest{};
    req.setUrl(QUrl{"http://127.0.0.1:27016"});
    auto reply = mHttpClient.get(req);

    QTRY_COMPARE_WITH_TIMEOUT(mHtppServer->handleRequestCalled(), true, timeout);
    QCOMPARE(mHtppServer->serverRequest().url(), QUrl{"/"});
    reply->deleteLater();
}

void ServerShould::give_a_request_with_the_request_body()
{
    auto req = QNetworkRequest{};
    req.setUrl(QUrl{"http://127.0.0.1:27016"});
    auto reply = mHttpClient.sendCustomRequest(req, "GET", QByteArray{"Hello World"});

    QTRY_COMPARE_WITH_TIMEOUT(mHtppServer->handleRequestCalled(), true, timeout);
    QCOMPARE(mHtppServer->serverRequest().body(), QByteArray{"Hello World"});
    reply->deleteLater();
}

} // namespace Http

QTEST_MAIN(Http::ServerShould)
