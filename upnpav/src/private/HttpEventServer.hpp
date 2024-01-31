// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Server.hpp"
#include "blabbyupnpav_export.h"
#include <QHostAddress>
#include <QObject>
#include <private/HttpEventSubscriptionHandle.hpp>

namespace UPnPAV
{

/**
 * The HttpEventServer is the communication endpoint for UPnPAV devices which send events for subscribed events.
 * There is only one instance in per process and all the @ref UPnPAV::HttpEventBackends are using the singleton
 * instance. It's possible to configure the @HttpEventServer IP address and port. The configuration is done with
 * environment variables: "BLABBY_EVENT_SERVER_IP" and "BLABBY_EVENT_SERVER_PORT"
 */
class BLABBYUPNPAV_EXPORT HttpEventServer final : public QObject
{
    Q_OBJECT
public:
    /**
     * Gives the HttpServerInstance
     */
    static HttpEventServer& instance();

    /**
     * Default destructor
     */
    ~HttpEventServer() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(HttpEventServer)

    /**
     * Gives the host address for the event callbacks.
     * @return The host address for the event callbacks.
     */
    QUrl callbackHost() noexcept;

    /**
     * Gives the raw server adress.
     * @retun The server address.
     */
    QHostAddress serverAddress() const noexcept;

private:
    /**
     * Default constructor
     */
    HttpEventServer();

private:
    friend struct HttpEventBackendPrivate;
    friend class HttpEventBackend;
    friend class HttpEventBackendShould;

    QHostAddress mServerAddress = QHostAddress{QHostAddress::QHostAddress::AnyIPv4};
    quint16 mServerPort = 0U;
    std::unique_ptr<Http::Server> mHttpServer = nullptr;
    QHash<ServiceDescription, QString> mRegisteredCallbacks;
};

} // namespace UPnPAV
