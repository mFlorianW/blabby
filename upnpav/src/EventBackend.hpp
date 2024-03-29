// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceDescription.hpp"
#include "blabbyupnpav_export.h"
#include <QHostAddress>
#include <QObject>
#include <QUrl>

namespace UPnPAV
{
/**
 * Forward declaration
 */
class EventSubscriptionHandle;
class EventSubscriptionCache;

/**
 * Parameters for sending a subscription request.
 */
struct EventSubscriptionParameters
{
    QString publisherPath;
    QString host;
    QString callback;
    quint32 timeout;

    bool operator==(EventSubscriptionParameters const& other) const noexcept = default;
};

/**
 * The @ref UPnPAV::EventBackend is the interface for a @ref UPnPAV::MediaDevice based classes for receiving and
 * subscribing UPnPAV events.
 * The @ref UPnPAV::EventBackend only handles the communication parts of the UPnPAV event mechanisim these are things
 * like sending the subscription requests, receiving subscribtion status and to provide the callback for the UPnPAV
 * devices to send events to a control point.
 * Every property change and managing the life of the subscription is done via the @ref EventSubscriptionHandle.
 */
class BLABBYUPNPAV_EXPORT EventBackend : public QObject
{
    Q_OBJECT
public:
    /**
     * Default Destrucotr
     */
    ~EventBackend() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(EventBackend)

    /**
     * Gives the callback host URL without any path for receiving events of UPnPAV devices.
     * @note
     * The URL will have most likly the form of http://<some_ip>:<port> or something like http//:<some_host>:<port>
     * @return The callback URL without any path for receiving events of UPnPAV devices.
     */
    virtual QUrl callbackHost() const noexcept = 0;

    /**
     * Registers the passed service for receiving events for that service.
     * This call doesn't subscribe for events it's just registers the callback in the backend.
     * The subscription for the events is done with @ref EventBackend::subscribeEvents.
     * If the service is already registered in the backend the already registered callback is used.
     * @return The eventcallback name, that can be used for the subscription.
     */
    virtual QString const& registerEventCallback(ServiceDescription const& desc) noexcept = 0;

    /**
     * Sends a event subscription request to a UPnPAV device for the passed @ref UPnPAV::ServiceDescription.
     *
     * @param host The destination for the subscription event.
     * @param params The subscription parameters to send a subscription to an UPnPAV device.
     * @return A handle for managing the subscription.
     */
    std::shared_ptr<EventSubscriptionHandle> subscribeEvents(ServiceDescription const& desc) noexcept;

protected:
    /**
     * Default constructor
     */
    EventBackend();

    /**
     * Sends a event subscription request to a UPnPAV device.
     *
     * @param params The subscription parameters to send a subscription to an UPnPAV device.
     * @return True subscription send, false on error.
     */
    virtual std::shared_ptr<EventSubscriptionHandle> sendSubscriptionRequest(
        EventSubscriptionParameters const& params) noexcept = 0;

    /**
     * This function is called by the backend to update the response body of the @ref UPnPAV::EventSubscriptionHandle
     * for the gived SID.
     * Concrete implementation should call the function when a UPnPAV device sends a property o
     * This will trigger the emit of the signal @ref UPnPAV::EventSubscriptionHandle::propertiesChanged.
     */
    void notificationReceived(QByteArray const& sid, QByteArray const& notificationData) noexcept;
};

/**
 * Error codes for the subscription handling
 */
enum class SubscriptionError
{
    /**
     * 400 Bad Request
     * If SID header and one of NT or CALLBACK headers are present, the publisher must respond with HTTP error 400 Bad
     * Request.
     */
    IncompatibleHeader = 400,

    /**
     * 412 Precondition Failed.
     * If CALLBACK header is missing or does not contain a valid HTTP URL, the publisher must respond with HTTP error
     * 412 Precondition Failed
     * If NT header does not equal upnp:event, the publisher must respond with HTTP error 412
     * Precondition Failed.
     */
    CallBackOrNtError = 412,

    /**
     * 5xx
     * If a publisher is not able to accept a subscription (such as due to insufficient resources), it must respond
     * with a HTTP 500-series error code.
     */
    UnableToAcceptSubscription = 500,
};

/**
 * A @ref UPnPAV::EventSubscriptionHandle manages an event subscription between an UPnPAV device and the control point.
 * The @ref UPnPAV::EventSubscriptionHandle notifies about the event subscription, incoming events and event timouts.
 * Every concret @ref UPnPAV::EventBackend must implement it's own @ref UPnPAV::EventSubscriptionHandle because the
 * @ref UPnPAV::EventBackend and @ref UPnPAV::EventSubscriptionHandle are very tidly coupled.
 * The implementations in the @ref UPnPAV::EventSubscriptionHandle are useable in every other concrete @ref
 * UPnPAV::EventSubscriptionHandle
 */
class BLABBYUPNPAV_EXPORT EventSubscriptionHandle : public QObject
{
    Q_OBJECT
public:
    /**
     * Default destructor
     */
    ~EventSubscriptionHandle() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(EventSubscriptionHandle)

    /**
     * Gives the current status about the subscription.
     * @return True the subsription is active otherwise false.
     */
    bool isSubscribed() const noexcept;

    /**
     * Gives subscription identifier for this handle
     * @return The subscription identifier.
     */
    QString const& sid() const noexcept;

    /**
     * Gives the response of the last received NOTIFY property update.
     * New data is only valid after receiving the @ref UPnPAV::EventSubscriptionHandle::propertiesChanged signal.
     * @return The last received NOTIFY data.
     */
    QString const& responseBody() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref UPnPAV::EventSubscriptionHandle is subscribed and received the positive
     * response.
     * @param handle Pointer to the handle which is now subscribed.
     */
    void subscribed(UPnPAV::EventSubscriptionHandle* handle);

    /**
     * This signal is emitted when the event subscription failes.
     * @param error The reason why the subscribing of events failed.
     */
    void subscriptionFailed(UPnPAV::SubscriptionError const& error);

    /**
     * This singal is emitted when the handle receives a NOTIFY message.
     * @param properties The body of the NOTIFY message.
     */
    void propertiesChanged();

    /**
     * This signal is emitted when the handle is not longer subscribed.
     * Used for internal purposes the signal should never be connected by clients.
     * param handle A pointer to the handle to identify the handle which is subsribed.
     * param QPrivateSignal This signal can only emitted by the class itself.
     */
    void unsubscribed(UPnPAV::EventSubscriptionHandle* handle, QPrivateSignal);

protected:
    /**
     * Creates an Instance of the @ref UPnPAV::EventSubscriptionHandle
     */
    EventSubscriptionHandle();

    /**
     * Sets the status of the subscription
     * @param isSubscribed True the subscription is active otherwise false.
     */
    void setIsSubscribed(bool isSubscribed) noexcept;

    /**
     * Sets the subscription identifier for this handle.
     * The subscription never changes during an active subscription.
     * If a subscription failes due to a timeout or something else then the events must be subscribed again.
     * @sid The subscription identifier for this handle.
     */
    void setSid(QString const& sid) noexcept;

    /**
     * Sets the response body for the @ref UPnPAV::EventSubscriptionHandle.
     * This typically done when a NOTIFY message from a device for this handle is received.
     * @param responseBody The response body of a NOTIFY message.
     */
    void setResponseBody(QString const& responseBody);

    /**
     * Starts the unsubscribe process for this subscription.
     * This function should never be called directly because the function is called by central @ref UPnPAV::EventBackend
     * cache, when a subscription is not longer needed.
     * The signal @ref UPnPAV::EventSubscriptionHandle::unsubscribed is emitted when unsubscribed.
     */
    virtual void unsubscribe(EventSubscriptionParameters const& params) = 0;

    /**
     * Allow subclasses to emit the private signal unsubscribed.
     */
    void emitUnsubscribed();

private:
    friend class UPnPAV::EventBackend;
    friend class UPnPAV::EventSubscriptionCache;
    bool mIsSubscribed = false;
    QString mSid;
    QString mResponseBody;
};

} // namespace UPnPAV

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug debug, UPnPAV::EventSubscriptionParameters const& params);
