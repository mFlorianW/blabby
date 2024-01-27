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

private:
    std::unique_ptr<HttpEventBackend> mEventBackend;
    std::unique_ptr<RequestHandler> mReceiver;
};

class RequestHandler : public Http::Server
{
    Q_OBJECT
public:
    RequestHandler();
    ~RequestHandler() override;
    Q_DISABLE_COPY_MOVE(RequestHandler)

    EventSubscriptionParameters lastSubscriptionRequest() const noexcept;

private:
    EventSubscriptionParameters mLastEventSubscriptionParams;
};

} // namespace UPnPAV
