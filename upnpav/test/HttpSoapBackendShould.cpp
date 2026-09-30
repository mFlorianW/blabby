// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapBackendShould.hpp"
#include "ClosedPort.hpp"
#include "ConnectionManagerActions.hpp"
#include "Descriptions.hpp"
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

void HttpSoapBackendShould::send_the_soap_action_in_double_quotes()
{
    auto device = Http::Server{};
    auto soapActions = QList<QByteArray>{};
    device.addRoute(QByteArrayLiteral("/control"),
                    [&soapActions](Http::ServerRequest const& request, Http::ServerResponse& response) {
                        // Header names are case-insensitive.
                        auto const& headers = request.headers();
                        for (auto header = headers.cbegin(); header != headers.cend(); ++header) {
                            if (header.key().compare("soapaction", Qt::CaseInsensitive) == 0) {
                                soapActions.append(header.value());
                            }
                        }
                        response.setStatusCode(Http::Response::StatusCode::InternalServerError);
                        return true;
                    });
    QVERIFY(device.listen(QHostAddress::LocalHost));
    auto backend = HttpSoapBackend{};
    auto const url = QStringLiteral("http://127.0.0.1:%1/control").arg(device.serverPort());
    auto const description = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                                QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
                                                url,
                                                url,
                                                url};
    auto scpd = validConnectionManagerSCPD();
    auto body = QString{};

    auto byUrl = backend.sendSoapMessage(url, QStringLiteral("Play"), QStringLiteral("urn:test"), QString{});
    auto byUrlSpy = QSignalSpy{byUrl.get(), &SoapCall::finished};
    QVERIFY(byUrlSpy.wait(TIMEOUT));
    auto byDescription = backend.sendSoapMessage(description, scpd, GetProtocolInfo(), body);
    auto byDescriptionSpy = QSignalSpy{byDescription.get(), &SoapCall::finished};
    QVERIFY(byDescriptionSpy.wait(TIMEOUT));

    QCOMPARE(soapActions,
             (QList<QByteArray>{
                 QByteArrayLiteral(R"("urn:test#Play")"),
                 QByteArrayLiteral(R"("urn:schemas-upnp-org:service:ConnectionManager:1#GetProtocolInfo")")}));
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::HttpSoapBackendShould)
