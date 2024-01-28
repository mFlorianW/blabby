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

EventSubscriptionParameters subscription(QString const& callback = QStringLiteral("<http://127.0.0.1:27015/callback0>"))
{
    static auto const params = EventSubscriptionParameters{
        .publisherPath = QStringLiteral("/test/eventUrl"),
        .host = "127.0.0.1:27016",
        .callback = callback,
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

void HttpEventBackendShould::receive_notify_requests()
{
    // activate subscription otherwise it's not possible to receive events
    auto handle = mEventBackend->subscribeEvents(validAvTransportServiceDescription());
    auto subscribeSpy = QSignalSpy{handle.get(), &EventSubscriptionHandle::subscribed};
    auto propChangeSpy = QSignalSpy{handle.get(), &EventSubscriptionHandle::propertiesChanged};

    QTRY_COMPARE_WITH_TIMEOUT(handle->isSubscribed(), true, 1000);
    QCOMPARE(subscribeSpy.size(), 1);

    auto mgr = QNetworkAccessManager{};
    auto request = QNetworkRequest{};
    auto const body = R"(<?xml version="1.0" encoding=" UTF-8"?><note></note>"))";
    auto content = QByteArray{body};
    request.setUrl(QUrl{"http://127.0.0.1:27015/callback0"});
    request.setRawHeader("HOST", "127.0.0.1:27016");
    request.setRawHeader("CONTENT-TYPE", "text/xml");
    request.setRawHeader("CONTENT-LENGTH", QString::number(content.size()).toUtf8());
    request.setRawHeader("NT", "upnp:event");
    request.setRawHeader("NTS", "upnp:propchange");
    request.setRawHeader("SID", handle->sid().toUtf8());
    request.setRawHeader("SEQ", "1");

    auto reply = mgr.sendCustomRequest(request, "NOTIFY", content);
    auto replySpy = QSignalSpy{reply, &QNetworkReply::finished};
    QTRY_COMPARE_WITH_TIMEOUT(replySpy.size(), 1, 1000);
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QCOMPARE(propChangeSpy.size(), 1);
    QCOMPARE(handle->responseBody(), body);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::HttpEventBackendShould)
