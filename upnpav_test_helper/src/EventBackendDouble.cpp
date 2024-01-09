// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "EventBackendDouble.hpp"

namespace UPnPAV::Doubles
{
class EventSubscriptionHandle : public UPnPAV::EventSubscriptionHandle
{
public:
    EventSubscriptionHandle(bool status)
    {
        setIsSubscribed(status);
    }
};

namespace
{

QString callbackId(QString const& serviceId)
{
    auto splittedServiceId = serviceId.split(":");
    if (splittedServiceId.size() < 4) {
        return "";
    }
    auto id = splittedServiceId.at(3);
    id.append(QStringLiteral("Callback"));

    return id;
}

} // namespace

EventBackend::EventBackend() = default;
EventBackend::~EventBackend() = default;

QUrl EventBackend::callbackHost() const noexcept
{
    return QUrl{QStringLiteral("http://127.0.0.1")};
}

QString const& EventBackend::registerEventCallback(ServiceDescription const& desc) noexcept
{
    auto const url = QUrl{desc.eventUrl()};
    if (not mCallbackRegistry.contains(url)) {
        auto const cbId = callbackId(desc.id());
        mCallbackRegistry.insert(url, cbId);
        mLastRegisteredCallbackId = cbId;
    }
    return mCallbackRegistry[url];
}

std::shared_ptr<UPnPAV::EventSubscriptionHandle> EventBackend::sendSubscriptionRequest(
    EventSubscriptionParameters const& params) noexcept
{
    auto const eventUrl = QString{"http://%1%2"}.arg(params.host, params.publisherPath);
    if (not mCallbackRegistry.contains(eventUrl)) {
        return std::make_shared<Doubles::EventSubscriptionHandle>(false);
    }

    mSubscribeRequest = params;
    return std::make_shared<Doubles::EventSubscriptionHandle>(true);
}

EventSubscriptionParameters EventBackend::lastSubscribeEventRequest() const noexcept
{
    return mSubscribeRequest;
}

QString const& EventBackend::lastRegisteredCallbackId() const noexcept
{
    return mLastRegisteredCallbackId;
}

} // namespace UPnPAV::Doubles
