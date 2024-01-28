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
    QString mEmptyString;
    HttpEventServer& mEventServer = HttpEventServer::instance();
    QHash<ServiceDescription, QString> mRegisteredCallbacks;
    QHash<QString, std::shared_ptr<HttpEventSubscriptionHandle>>& mSubscriptions = mEventServer.mSubscriptions;
    QHash<QObject*, std::shared_ptr<HttpEventSubscriptionHandle>> mPendingSubscriptions;
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
    auto handle = std::make_shared<HttpEventSubscriptionHandle>(d->mEventServer.serverAddress());
    connect(handle.get(), &HttpEventSubscriptionHandle::subscribed, this, [this]() {
        if (d->mPendingSubscriptions.contains(sender())) {
            auto handle = d->mPendingSubscriptions.value(sender());
            d->mSubscriptions.insert(handle->sid(), handle);
        }
    });
    handle->subscribe(params);
    d->mPendingSubscriptions.insert(handle.get(), handle);
    return handle;
}

bool HttpEventBackend::onNotifyReceived(Http::ServerRequest const& request, Http::ServerResponse& response) noexcept
{
    auto const sid = request.headers().value("SID");
    if (d->mSubscriptions.contains(sid)) {
        response.setStatusCode(Http::Response::StatusCode::Ok);
        d->mSubscriptions.value(sid)->setBody(request.body());
        return true;
    }

    response.setStatusCode(Http::Response::StatusCode::NotFound);
    return false;
}

} // namespace UPnPAV
