// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "EventBackend.hpp"
#include <QUrl>

namespace UPnPAV::Doubles
{

/**
 * EventBackendDouble for testing purposes.
 */
class EventBackend : public UPnPAV::EventBackend
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    EventBackend();

    /**
     * Default destructor
     */
    ~EventBackend() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(EventBackend)

    /**
     * @copydoc UPnPAV::EventBackend::callbackHost
     */
    QUrl callbackHost() const noexcept override;

    /**
     * @copydoc UPnPAV::EventBackend::registerEventCallback
     */
    QString const& registerEventCallback(ServiceDescription const& desc) noexcept override;

    /**
     * Gives the parameters of the last @ref UPnPAV::Doubles::EventBackend::subscribeEvents all.
     * @return The parameters of the @ref UPnPAV::Doubles::EventBackend::subscribeEvents call:
     */
    EventSubscriptionParameters lastSubscribeEventRequest() const noexcept;

    /**
     * Gives the last registered callback ID.
     * @return The last registered call ID.
     */
    QString const& lastRegisteredCallbackId() const noexcept;

protected:
    /**
     * @copydoc UPnPAV::EventBackend::subscribeEvents
     */
    std::shared_ptr<EventSubscriptionHandle> sendSubscriptionRequest(
        EventSubscriptionParameters const& params) noexcept override;

private:
    EventSubscriptionParameters mSubscribeRequest;
    QString mLastRegisteredCallbackId;
    QHash<QUrl, QString> mCallbackRegistry;
};

} // namespace UPnPAV::Doubles
