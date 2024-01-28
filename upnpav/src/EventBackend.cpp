// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "EventBackend.hpp"

namespace UPnPAV
{

EventBackend::EventBackend() = default;
EventBackend::~EventBackend() = default;

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
            Q_EMIT subscribed();
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

std::shared_ptr<EventSubscriptionHandle> EventBackend::subscribeEvents(ServiceDescription const& desc) noexcept
{
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

    return sendSubscriptionRequest(params);
}

void EventSubscriptionHandle::setResponseBody(QString const& responseBody)
{
    if (mResponseBody != responseBody) {
        mResponseBody = responseBody;
        Q_EMIT propertiesChanged();
    }
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
