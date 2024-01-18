// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServerShould.hpp"
#include <QTest>

namespace Http
{

bool TestServer::handleRequestCalled() const noexcept
{
    return mHandleRequestCalled;
}

bool TestServer::handleRequest(ServerRequest const& request, ServerResponse& response)
{
    Q_UNUSED(request);
    Q_UNUSED(response);

    mHandleRequestCalled = true;

    return true;
}

ServerShould::~ServerShould() = default;

void ServerShould::init()
{
    mHtppServer = std::make_unique<TestServer>();
    QCOMPARE(mHtppServer->bind(QHostAddress::LocalHost, 27016), true);
}

void ServerShould::call_handle_request_on_susccessful_http_request()
{
    auto req = QNetworkRequest{};
    req.setUrl(QUrl{"http://127.0.0.1:27016"});
    mHttpClient.get(req);

    QTRY_COMPARE_WITH_TIMEOUT(mHtppServer->handleRequestCalled(), true, 100);
}

} // namespace Http

QTEST_MAIN(Http::ServerShould)
