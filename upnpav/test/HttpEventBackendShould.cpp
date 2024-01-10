// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventBackendShould.hpp"
#include <QTest>
#include <QUrl>

namespace UPnPAV
{

HttpEventBackendShould::~HttpEventBackendShould() = default;

void HttpEventBackendShould::initTestCase()
{
    qputenv("BLABBY_EVENT_SERVER_IP", QHostAddress{QHostAddress::LocalHost}.toString().toLocal8Bit());
    qputenv("BLABBY_EVENT_SERVER_PORT", "27015");
}

void HttpEventBackendShould::init()
{
    mEventBackend = std::make_unique<HttpEventBackend>();
}

void HttpEventBackendShould::give_callback_host_url()
{
    auto const cbHost = mEventBackend->callbackHost();
    QCOMPARE(cbHost, QUrl{"http://127.0.0.1:27015"});
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::HttpEventBackendShould)
