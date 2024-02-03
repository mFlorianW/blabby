// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "EventBackend.hpp"
#include "ServerRequest.hpp"
#include "ServerResponse.hpp"
#include "blabbyupnpav_export.h"
#include <QTcpSocket>

namespace UPnPAV
{
class HttpEventServer;
struct HttpEventBackendPrivate;

/**
 * The HttpEventBackend is the HTTP based implementation of the @ref UPnPAV::EventBackend interface.
 */
class BLABBYUPNPAV_EXPORT HttpEventBackend : public EventBackend
{
    Q_OBJECT
public:
    /**
     * Creates a HttpEventBackend instance.
     */
    HttpEventBackend();

    /**
     * Default destructor
     */
    ~HttpEventBackend() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(HttpEventBackend)

    /**
     * @copydoc UPnPAV::EventBackend::callbackHost
     */
    QUrl callbackHost() const noexcept override;

    /**
     * @copydoc UPnPAV::EventBackend::registerEventCallback
     */
    QString const& registerEventCallback(ServiceDescription const& desc) noexcept override;

protected:
    /**
     * @copydoc UPnPAV::EventBackend::subscribeEvents
     */
    std::shared_ptr<EventSubscriptionHandle> sendSubscriptionRequest(
        EventSubscriptionParameters const& params) noexcept override;

private:
    bool onNotifyReceived(Http::ServerRequest const& request, Http::ServerResponse& response) noexcept;

private:
    std::unique_ptr<HttpEventBackendPrivate> d;
};

} // namespace UPnPAV
