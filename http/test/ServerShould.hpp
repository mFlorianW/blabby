// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Server.hpp"
#include <QNetworkAccessManager>
#include <QObject>

namespace Http
{

class TestServer;

class ServerShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ServerShould() override;
    Q_DISABLE_COPY_MOVE(ServerShould)

private Q_SLOTS:
    /**
     * Setups a new server for the tests.
     */
    void initTestCase();

    /**
     * Reset server internal state for every test.
     */
    void init();

    void give_a_request_with_the_request_method_data();
    /**
     * Tests that the server returns the request method to the handle request function
     */
    void give_a_request_with_the_request_method();

    /**
     * Tests that the server correctly parse the request from the raw HTTP request.
     */
    void give_a_request_with_the_headers();

    /**
     * Tests that the server correctly parse the URL from the raw HTTP request.
     */
    void give_a_request_with_the_request_url();

    /**
     * Tests that the server correctly parse the body from the raw HTTP request.
     */
    void give_a_request_with_the_request_body();

private:
    std::unique_ptr<TestServer> mHtppServer;
    QNetworkAccessManager mHttpClient;
};

class TestServer : public Server
{
public:
    bool handleRequestCalled() const noexcept;
    ServerRequest serverRequest() const noexcept;
    void resetStates() noexcept;

protected:
    bool handleRequest(ServerRequest const& request, ServerResponse& response) override;

private:
    bool mHandleRequestCalled = false;
    ServerRequest mLastRequest;
};

} // namespace Http
