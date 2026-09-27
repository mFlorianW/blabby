// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventSubscriptionHandle.hpp"
#include "private/LoggingCategories.hpp"
#include <QCoreApplication>
#include <QUrl>

namespace UPnPAV
{

namespace
{
/**
 * Upper bound in milliseconds for an UNSUBSCRIBE request, a device that doesn't answer must not keep the handle alive
 * forever.
 */
constexpr auto UnsubscribeTimeoutMs = 5000;

/**
 * Aborts the reply without triggering the slots of the receiver, the reply is deleted by its QNetworkAccessManager.
 */
void abortSilently(QNetworkReply* reply, QObject const* receiver)
{
    if (reply != nullptr) {
        QObject::disconnect(reply, nullptr, receiver, nullptr);
        reply->abort();
    }
}
} // namespace

HttpEventSubscriptionHandle::HttpEventSubscriptionHandle(QHostAddress hostAddress)
    : EventSubscriptionHandle{}
    , mHostAddress{hostAddress}
{
    connect(&mSubscriptionRenewTimer, &QTimer::timeout, this, [this] {
        subscribe(mParams);
    });
}

HttpEventSubscriptionHandle::~HttpEventSubscriptionHandle()
{
    // Abort the pending requests before the members are destroyed, so that no reply slot runs on a partially
    // destroyed handle. The QNetworkAccessManager member then waits for its worker thread when it is destroyed.
    mSubscriptionRenewTimer.stop();
    abortSilently(mSubscribeRequestPending, this);
    abortSilently(mUnsubscribeRequestPending, this);
}

void HttpEventSubscriptionHandle::subscribe(EventSubscriptionParameters const& params) noexcept
{
    mParams = params;
    if (mSubscribeRequestPending != nullptr || mUnsubscribeRequestPending != nullptr) {
        return;
    }

    auto req = QNetworkRequest{QUrl{QString{"http://%1%2"}.arg(params.host, params.publisherPath)}};
    req.setRawHeader(QByteArray{"HOST"}, params.host.toUtf8());
    req.setRawHeader(QByteArray{"CALLBACK"}, params.callback.toUtf8());
    req.setRawHeader(QByteArray{"NT"}, QByteArray{"upnp:event"});
    req.setRawHeader(QByteArray{"TIMEOUT"}, QString{"Second-%1"}.arg(QString::number(params.timeout)).toUtf8());

    auto* reply = mNetworkManager.sendCustomRequest(req, QByteArray{"SUBSCRIBE"});
    mSubscribeRequestPending = reply;

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        mSubscribeRequestPending = nullptr;
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qCCritical(upnpavEvent) << "Event subscription" << reply->url() << "failed. Error:" << reply->error();
            return;
        }

        auto rawTimeout = reply->rawHeader("TIMEOUT");
        auto indexOfDash = rawTimeout.indexOf("-");
        bool conversionOk = false;
        auto timeout = static_cast<quint32>(rawTimeout.remove(0, indexOfDash + 1).toFloat(&conversionOk) / 2 * 1000);
        if (not conversionOk) {
            Q_EMIT subscriptionFailed(SubscriptionError::IncompatibleHeader);
            return;
        }
        mSubscriptionRenewTimer.setInterval(std::chrono::milliseconds{timeout});
        mSubscriptionRenewTimer.start();
        setSid(reply->rawHeader(QByteArray{"SID"}));
        setIsSubscribed(true);
    });
}

void HttpEventSubscriptionHandle::unsubscribe(EventSubscriptionParameters const& params) noexcept
{
    if (mUnsubscribeRequestPending != nullptr) {
        return;
    }

    mSubscriptionRenewTimer.stop();
    abortSilently(mSubscribeRequestPending, this);
    mSubscribeRequestPending = nullptr;

    // Without a subscription there is no SID and nothing to unsubscribe.
    if (not isSubscribed()) {
        emitUnsubscribed();
        return;
    }

    auto req = QNetworkRequest{QUrl{QString{"http://%1%2"}.arg(params.host, params.publisherPath)}};
    req.setRawHeader(QByteArray{"HOST"}, params.host.toUtf8());
    req.setRawHeader(QByteArray{"SID"}, sid().toUtf8());
    req.setTransferTimeout(UnsubscribeTimeoutMs);

    auto* reply = mNetworkManager.sendCustomRequest(req, QByteArray{"UNSUBSCRIBE"});
    mUnsubscribeRequestPending = reply;

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        mUnsubscribeRequestPending = nullptr;
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(upnpavEvent) << "Event unsubscribe" << reply->url() << "failed. Error:" << reply->error();
        }
        emitUnsubscribed();
    });
}

void HttpEventSubscriptionHandle::setBody(QString const& body) noexcept
{
    setResponseBody(body);
}

void HttpEventSubscriptionHandleDeleter::operator()(HttpEventSubscriptionHandle* handle)
{
    // The handle outlives its owners until the unsubscribe is finished. Parent it to the application, so that it is
    // destroyed together with the application at the latest, even when the event loop doesn't run anymore to handle
    // the deleteLater. Otherwise its network requests would still be running while the process exits.
    handle->setParent(QCoreApplication::instance());
    QObject::connect(handle, &HttpEventSubscriptionHandle::unsubscribed, handle, [handle]() {
        handle->deleteLater();
    });
    handle->unsubscribe(handle->mParams);
}

} // namespace UPnPAV
