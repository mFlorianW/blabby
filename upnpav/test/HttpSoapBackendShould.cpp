// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapBackendShould.hpp"
#include "ClosedPort.hpp"
#include "HttpSoapBackend.hpp"
#include "Server.hpp"
#include <QSignalSpy>
#include <QTest>

namespace UPnPAV
{

namespace
{

constexpr auto TIMEOUT = 5000;

} // namespace

HttpSoapBackendShould::~HttpSoapBackendShould() = default;

void HttpSoapBackendShould::tell_that_the_device_is_unreachable_when_nobody_answers()
{
    auto backend = HttpSoapBackend{};
    auto const url = QStringLiteral("http://127.0.0.1:%1/control").arg(closedPort());

    auto call = backend.sendSoapMessage(url, QStringLiteral("Play"), QStringLiteral("urn:test"), QString{});
    auto finishedSpy = QSignalSpy{call.get(), &SoapCall::finished};

    QVERIFY(finishedSpy.wait(TIMEOUT));
    QCOMPARE(call->hasFinishedSuccesful(), false);
    QCOMPARE(call->isDeviceUnreachable(), true);
}

void HttpSoapBackendShould::not_tell_that_the_device_is_unreachable_when_it_answers_with_an_error()
{
    auto device = Http::Server{};
    device.addRoute(QByteArrayLiteral("/control"), [](Http::ServerRequest const&, Http::ServerResponse& response) {
        response.setStatusCode(Http::Response::StatusCode::InternalServerError);
        return true;
    });
    QVERIFY(device.listen(QHostAddress::LocalHost));
    auto backend = HttpSoapBackend{};
    auto const url = QStringLiteral("http://127.0.0.1:%1/control").arg(device.serverPort());

    auto call = backend.sendSoapMessage(url, QStringLiteral("Play"), QStringLiteral("urn:test"), QString{});
    auto finishedSpy = QSignalSpy{call.get(), &SoapCall::finished};

    QVERIFY(finishedSpy.wait(TIMEOUT));
    QCOMPARE(call->hasFinishedSuccesful(), false);
    QCOMPARE(call->isDeviceUnreachable(), false);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::HttpSoapBackendShould)
