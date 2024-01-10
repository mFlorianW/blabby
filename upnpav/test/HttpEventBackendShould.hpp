// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpEventBackend.hpp"
#include <QObject>

namespace UPnPAV
{

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

private:
    std::unique_ptr<HttpEventBackend> mEventBackend;
};

} // namespace UPnPAV
