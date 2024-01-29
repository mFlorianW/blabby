// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventBackend.hpp"
#include "private/HttpEventServer.hpp"
#include "private/HttpEventSubscriptionHandle.hpp"
#include <QUrl>
#include <algorithm>

namespace UPnPAV
{

struct HttpEventBackendPrivate
{
    QString mEmptyString;
    HttpEventServer& mEventServer = HttpEventServer::instance();
    QHash<ServiceDescription, QString>& mRegisteredCallbacks = mEventServer.mRegisteredCallbacks;
    QHash<QString, std::shared_ptr<HttpEventSubscriptionHandle>>& mSubscriptions = mEventServer.mSubscriptions;
    std::vector<HttpEventServer::PendingSubscription>& mPendingSubscriptions = mEventServer.mPendingSubscriptions;

    bool isPending(EventSubscriptionParameters const& params) const noexcept
    {
        return std::find_if(mPendingSubscriptions.cbegin(),
                            mPendingSubscriptions.cend(),
                            [&params](HttpEventServer::PendingSubscription const& sub) {
                                return sub.params == params;
                            }) != mPendingSubscriptions.cend();
    }

    bool isPending(QObject* handle) const noexcept
    {
        return std::find_if(mPendingSubscriptions.cbegin(),
                            mPendingSubscriptions.cend(),
                            [&handle](HttpEventServer::PendingSubscription const& sub) {
                                return sub.handle.get() == handle;
                            }) != mPendingSubscriptions.cend();
    }

    std::shared_ptr<HttpEventSubscriptionHandle> pendingSubscriptionRequest(
        EventSubscriptionParameters const& params) const noexcept
    {
        auto iter = std::find_if(mPendingSubscriptions.cbegin(),
                                 mPendingSubscriptions.cend(),
                                 [&params](HttpEventServer::PendingSubscription const& sub) {
                                     return sub.params == params;
                                 });
        return iter->handle;
    }

    std::shared_ptr<HttpEventSubscriptionHandle> pendingSubscriptionRequest(QObject* handle) const noexcept
    {
        auto iter = std::find_if(mPendingSubscriptions.cbegin(),
                                 mPendingSubscriptions.cend(),
                                 [&handle](HttpEventServer::PendingSubscription const& sub) {
                                     return sub.handle.get() == handle;
                                 });
        return iter->handle;
    }

    void removePendingRequest(QObject* handle)
    {
        mPendingSubscriptions.erase(mPendingSubscriptions.begin(),
                                    std::remove_if(mPendingSubscriptions.begin(),
                                                   mPendingSubscriptions.end(),
                                                   [&handle](HttpEventServer::PendingSubscription const& sub) {
                                                       return sub.handle.get() == handle;
                                                   }));
    }
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
    if (not d->isPending(params)) {
        auto handle = std::make_shared<HttpEventSubscriptionHandle>(d->mEventServer.serverAddress());
        connect(handle.get(), &HttpEventSubscriptionHandle::subscribed, this, [this]() {
            auto* rawHandle = sender();
            if (d->isPending(rawHandle)) {
                auto handle = d->pendingSubscriptionRequest(rawHandle);
                d->mSubscriptions.insert(handle->sid(), handle);
                d->removePendingRequest(rawHandle);
            }
        });
        handle->subscribe(params);
        d->mPendingSubscriptions.emplace_back(handle, params);
        return handle;
    }

    return d->pendingSubscriptionRequest(params);
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
