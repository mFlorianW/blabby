// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "EventBackend.hpp"
#include "HttpEventBackend.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QTcpSocket>

namespace UPnPAV
{

/**
 * HTTP implementation of the @ref UPnPAV::EventSubscriptionHandle
 */
class HttpEventSubscriptionHandle final : public EventSubscriptionHandle
{
    Q_OBJECT
public:
    /**
     * Creates an Instance of the HttpEventSubscriptionHandle
     * @param hostAddress The host address that is used for the communication with UPnPAV device.
     */
    HttpEventSubscriptionHandle(QHostAddress hostAddress);

    /**
     * Default destructor
     */
    ~HttpEventSubscriptionHandle() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(HttpEventSubscriptionHandle)

protected:
    /**
     * Tries to subscribes the events defined in the msgBody
     * @param host The UPnPAV device host address
     * @param params The parameters for subscribing the event of a UPnPAV device.
     *
     * @note The @ref UPnPAV::EventSubscriptionHandle::subscribed signal is emitted on succesful event subscription.
     */
    void subscribe(EventSubscriptionParameters const& params) noexcept;

private:
    void setBody(QString const& body) noexcept;

private:
    friend class UPnPAV::HttpEventBackend;
    QHostAddress mHostAddress;
    QNetworkAccessManager mNetworkManager;
    QNetworkReply* mSubscribeRequestPending = nullptr;
};

}; // namespace UPnPAV
