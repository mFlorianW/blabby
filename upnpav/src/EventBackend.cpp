// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "EventBackend.hpp"

namespace UPnPAV
{

struct EventSubscriptionCacheEntry
{
    std::weak_ptr<EventSubscriptionHandle> mHandle;
    EventSubscriptionParameters mParams;
    ServiceDescription mDesc;
    QString mSid = QString{""};
};

class EventSubscriptionCache final
{
public:
    EventSubscriptionCache() = default;
    ~EventSubscriptionCache() = default;

    Q_DISABLE_COPY_MOVE(EventSubscriptionCache)

    static EventSubscriptionCache& instance()
    {
        static EventSubscriptionCache e;
        return e;
    }

    QHash<QObject*, EventSubscriptionCacheEntry> mActiveSubscriptions;
    QHash<QObject*, EventSubscriptionCacheEntry> mPendingSubscriptionRequests;
    QHash<QObject*, EventSubscriptionCacheEntry> mPendingUnsubscribeRequests;

    std::optional<std::shared_ptr<EventSubscriptionHandle>> serviceSubscribed(
        ServiceDescription const& desc) const noexcept
    {
        auto const activeSub = std::find_if(mActiveSubscriptions.cbegin(),
                                            mActiveSubscriptions.cend(),
                                            [&desc](EventSubscriptionCacheEntry const& entry) {
                                                return entry.mDesc == desc;
                                            });
        if (activeSub != mActiveSubscriptions.cend() && not activeSub->mHandle.expired()) {
            return activeSub->mHandle.lock();
        }
        auto const pendingSub = std::find_if(mPendingSubscriptionRequests.cbegin(),
                                             mPendingSubscriptionRequests.cend(),
                                             [&desc](EventSubscriptionCacheEntry const& entry) {
                                                 return entry.mDesc == desc;
                                             });
        if (pendingSub != mPendingSubscriptionRequests.cend() && not pendingSub->mHandle.expired()) {
            return pendingSub->mHandle.lock();
        }
        return std::nullopt;
    }

    std::optional<std::shared_ptr<EventSubscriptionHandle>> handle(QByteArray const& sid) const noexcept
    {
        auto const activeSub = std::find_if(mActiveSubscriptions.cbegin(),
                                            mActiveSubscriptions.cend(),
                                            [&sid](EventSubscriptionCacheEntry const& entry) {
                                                return entry.mSid == sid;
                                            });
        if (activeSub != mActiveSubscriptions.cend() && not activeSub->mHandle.expired()) {
            return activeSub->mHandle.lock();
        }
        return std::nullopt;
    }

    void cleanup()
    {
        auto iter = mActiveSubscriptions.cbegin();
        while (iter != mActiveSubscriptions.cend()) {
            if (iter.value().mHandle.expired()) {
                iter = QHash<QObject*, EventSubscriptionCacheEntry>::const_iterator{mActiveSubscriptions.erase(iter)};
            } else {
                ++iter;
            }
        }
    }
};

EventBackend::EventBackend() = default;

EventBackend::~EventBackend()
{
    EventSubscriptionCache::instance().cleanup();
};

std::shared_ptr<EventSubscriptionHandle> EventBackend::subscribeEvents(ServiceDescription const& desc) noexcept
{
    auto& eventCache = EventSubscriptionCache::instance();
    auto serviceSubscribed = eventCache.serviceSubscribed(desc);
    if (serviceSubscribed) {
        return serviceSubscribed.value();
    }

    auto const url = QUrl{desc.eventUrl()};
    auto const publisher = url.path();
    auto const cEndp = registerEventCallback(desc);
    auto const callback = QString{"<%1%2>"}.arg(callbackHost().toString(), cEndp);
    auto host = url.host();
    auto const port = url.port();
    if (port != -1) {
        auto const portParm = QString{":%1"}.arg(QString::number(port));
        host.append(portParm);
    }

    auto params = EventSubscriptionParameters{
        .publisherPath = publisher,
        .host = host,
        .callback = callback,
        .timeout = 1800,
    };

    auto const handle = sendSubscriptionRequest(params);
    connect(handle.get(), &EventSubscriptionHandle::subscribed, this, [&](EventSubscriptionHandle* handle) {
        if (eventCache.mPendingSubscriptionRequests.contains(handle)) {
            auto entry = eventCache.mPendingSubscriptionRequests.value(handle);
            entry.mSid = handle->sid();
            eventCache.mActiveSubscriptions.insert(handle, entry);
            eventCache.mPendingSubscriptionRequests.remove(handle);
        }
    });
    EventSubscriptionCache::instance().mPendingSubscriptionRequests.insert(
        handle.get(),
        {.mHandle = handle, .mParams = params, .mDesc = desc});
    if (handle->isSubscribed()) {
        // This for the the handle is already connected.
        Q_EMIT handle->subscribed(handle.get());
    }

    return handle;
}

void EventBackend::notificationReceived(QByteArray const& sid, QByteArray const& notificationData) noexcept
{
    auto const& eventCache = EventSubscriptionCache::instance();
    auto const handle = eventCache.handle(sid);
    if (handle) {
        handle.value()->setResponseBody(QString{notificationData});
    }
}

EventSubscriptionHandle::EventSubscriptionHandle() = default;
EventSubscriptionHandle::~EventSubscriptionHandle() = default;

bool EventSubscriptionHandle::isSubscribed() const noexcept
{
    return mIsSubscribed;
}

void EventSubscriptionHandle::setIsSubscribed(bool isSubscribed) noexcept
{
    if (mIsSubscribed != isSubscribed) {
        mIsSubscribed = isSubscribed;
        if (mIsSubscribed) {
            Q_EMIT subscribed(this);
        }
    }
}

QString const& EventSubscriptionHandle::sid() const noexcept
{
    return mSid;
}

void EventSubscriptionHandle::setSid(QString const& sid) noexcept
{
    mSid = sid;
}

QString const& EventSubscriptionHandle::responseBody() const noexcept
{
    return mResponseBody;
}

void EventSubscriptionHandle::setResponseBody(QString const& responseBody)
{
    if (mResponseBody != responseBody) {
        mResponseBody = responseBody;
        Q_EMIT propertiesChanged();
    }
}
void EventSubscriptionHandle::emitUnsubscribed()
{
    Q_EMIT unsubscribed(this, QPrivateSignal{});
}

} // namespace UPnPAV

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug debug, UPnPAV::EventSubscriptionParameters const& params)
{
    auto saver = QDebugStateSaver{debug};

    debug << "publisher path:" << params.publisherPath;
    debug << "host:" << params.host;
    debug << "callback:" << params.callback;
    debug << "timeout:" << params.timeout;

    return debug;
}
