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
     * Setups a new server for each test.
     */
    void init();

    /**
     * Tests that the handle request function is called when a http request is successful
     * received.
     */
    void call_handle_request_on_susccessful_http_request();

private:
    std::unique_ptr<TestServer> mHtppServer;
    QNetworkAccessManager mHttpClient;
};

class TestServer : public Server
{
public:
    bool handleRequestCalled() const noexcept;

protected:
    bool handleRequest(ServerRequest const& request, ServerResponse& response) override;

private:
    bool mHandleRequestCalled = false;
};

} // namespace Http
