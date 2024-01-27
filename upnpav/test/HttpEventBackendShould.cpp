// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventBackendShould.hpp"
#include "Descriptions.hpp"
#include <QSignalSpy>
#include <QTcpServer>
#include <QTest>
#include <QUrl>

namespace UPnPAV
{
namespace
{

EventSubscriptionParameters subscription()
{
    static auto const params = EventSubscriptionParameters{
        .publisherPath = QStringLiteral("/test/eventUrl"),
        .host = "127.0.0.1:27016",
        .callback = QStringLiteral("<http://127.0.0.1:27015/callback0>"),
        .timeout = 1800,
    };
    return params;
}

} // namespace

RequestHandler::RequestHandler()
    : Http::Server{}
{
    addRoute("/test/eventUrl", [this](Http::ServerRequest const& request, Http::ServerResponse& resp) -> bool {
        if (request.method() != Http::Request::Method::Subscribe) {
            resp.setStatusCode(Http::Response::StatusCode::NotFound);
            return false;
        }

        mLastEventSubscriptionParams.publisherPath = request.url().toString();
        mLastEventSubscriptionParams.host = request.headers().value("HOST");
        mLastEventSubscriptionParams.callback = request.headers().value("CALLBACK");

        auto time = request.headers().value("TIMEOUT");
        const auto indexOfDash = time.indexOf("-");
        mLastEventSubscriptionParams.timeout = time.remove(0, indexOfDash + 1).toInt();

        // send the subscription ID back.
        resp.setStatusCode(Http::Response::StatusCode::Ok);
        resp.setHeader("SID", "uuid:12345");
        return true;
    });
}

RequestHandler::~RequestHandler() = default;

EventSubscriptionParameters RequestHandler::lastSubscriptionRequest() const noexcept
{
    return mLastEventSubscriptionParams;
}

HttpEventBackendShould::~HttpEventBackendShould() = default;

void HttpEventBackendShould::initTestCase()
{
    qputenv("BLABBY_EVENT_SERVER_IP", QHostAddress{QHostAddress::LocalHost}.toString().toLocal8Bit());
    qputenv("BLABBY_EVENT_SERVER_PORT", "27015");

    mReceiver = std::make_unique<RequestHandler>();
    QCOMPARE(mReceiver->listen(QHostAddress::LocalHost, 27016), true);
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

void HttpEventBackendShould::send_subscription_request_correctly()
{
    auto handle = mEventBackend->subscribeEvents(validAvTransportServiceDescription());
    auto subscribeSpy = QSignalSpy{handle.get(), &EventSubscriptionHandle::subscribed};

    QTRY_COMPARE_WITH_TIMEOUT(handle->isSubscribed(), true, 1000);
    QCOMPARE(mReceiver->lastSubscriptionRequest(), subscription());
    QCOMPARE(subscribeSpy.size(), 1);
    QCOMPARE(handle->sid(), QStringLiteral("uuid:12345"));
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::HttpEventBackendShould)
