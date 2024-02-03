// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventBackend.hpp"
#include "private/HttpEventServer.hpp"
#include "private/HttpEventSubscriptionHandle.hpp"
#include <QUrl>

namespace UPnPAV
{

struct HttpEventBackendPrivate
{
    HttpEventServer& mEventServer = HttpEventServer::instance();
    QHash<ServiceDescription, QString>& mRegisteredCallbacks = mEventServer.mRegisteredCallbacks;
};

HttpEventBackend::HttpEventBackend()
    : EventBackend{}
    , d{std::make_unique<HttpEventBackendPrivate>()}
{
}

HttpEventBackend::~HttpEventBackend() = default;

QUrl HttpEventBackend::callbackHost() const noexcept
{
    return d->mEventServer.callbackHost();
}

QString const& HttpEventBackend::registerEventCallback(ServiceDescription const& desc) noexcept
{
    if (d->mRegisteredCallbacks.contains(desc)) {
        return d->mRegisteredCallbacks[desc];
    }

    auto callback = QString{"/callback%1"}.arg(QString::number(d->mRegisteredCallbacks.size()));
    d->mRegisteredCallbacks.insert(desc, callback);
    d->mEventServer.mHttpServer->addRoute(callback.toUtf8(), [this](auto&& request, auto&& response) {
        return onNotifyReceived(std::forward<decltype(request)>(request), std::forward<decltype(response)>(response));
    });
    return d->mRegisteredCallbacks[desc];
}

std::shared_ptr<EventSubscriptionHandle> HttpEventBackend::sendSubscriptionRequest(
    EventSubscriptionParameters const& params) noexcept
{
    auto handle = std::shared_ptr<HttpEventSubscriptionHandle>(
        new (std::nothrow) HttpEventSubscriptionHandle{d->mEventServer.serverAddress()},
        HttpEventSubscriptionHandleDeleter{});
    handle->subscribe(params);
    return handle;
}

bool HttpEventBackend::onNotifyReceived(Http::ServerRequest const& request, Http::ServerResponse& response) noexcept
{
    auto const sid = request.headers().value("SID");
    auto const body = request.body();

    notificationReceived(sid, body);

    response.setStatusCode(Http::Response::StatusCode::Ok);
    return true;
}

} // namespace UPnPAV
