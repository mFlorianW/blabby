// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpEventBackend.hpp"
#include "Server.hpp"
#include <QObject>

namespace UPnPAV
{
class RequestHandler;

class HttpEventBackendShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~HttpEventBackendShould() override;
    Q_DISABLE_COPY_MOVE(HttpEventBackendShould)

private Q_SLOTS:
    /**
     * Initialized the test suite, sets env variables so the HTTP event server is only listening on localhost
     * and on a specific port for testing reasons.
     */
    void initTestCase();

    /**
     * Initialized the HttpEventBackend before each test run.
     */
    void init();

    /**
     * @test
     * Tests that the event backend returns the correct host address.
     */
    void give_callback_host_url();

    /**
     * @test
     * Tests that subscription requests are correctly sent.
     */
    void send_subscription_request_correctly();

    /**
     * @test
     * Tests that the @ref UPnPAV::HttpEventBackend correctly receives NOTIFY requests.
     */
    void receive_notify_requests();

    /**
     * @test
     * Tests that the @ref UPnPAV::HttpEventBackend gives always the same @ref UPnPAV::EventSubscriptionHandle
     * even when the subscription is not active.
     */
    void always_return_the_same_event_handle_for_the_same_service();

    /**
     * @test
     * Tests that the @ref UPnPAV::HttpEventBackend correctly unsubscribe the events correctly when these
     * are not longer needed.
     */
    void unsubscribe_events_when_not_longer_needed();

    /**
     * @test
     * Tests that the subscription created by the @ref UPnPAV::HttpEventBackend renewed in required timeout frame.
     */
    void renew_the_subscription();

private:
    std::unique_ptr<HttpEventBackend> mEventBackend;
    std::unique_ptr<RequestHandler> mReceiver;
};

struct UnsubscribeRequestParameters
{
    QString publisherPath;
    QString host;
    QString sid;
};

class RequestHandler : public Http::Server
{
    Q_OBJECT
public:
    RequestHandler();
    ~RequestHandler() override;
    Q_DISABLE_COPY_MOVE(RequestHandler)

    EventSubscriptionParameters lastSubscriptionRequest() const noexcept;
    void clearLastSubscriptionRequest() noexcept;

    UnsubscribeRequestParameters lastUnsubscribeRequest() const noexcept;

private:
    EventSubscriptionParameters mLastEventSubscriptionParams;
    UnsubscribeRequestParameters mLastUnsubscribeParams;
};

} // namespace UPnPAV
