// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventSubscriptionHandle.hpp"
#include "private/LoggingCategories.hpp"
#include <QUrl>

namespace UPnPAV
{

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
    if (mSubscribeRequestPending) {
        mSubscribeRequestPending->deleteLater();
    }
}

void HttpEventSubscriptionHandle::subscribe(EventSubscriptionParameters const& params) noexcept
{
    auto req = QNetworkRequest{QUrl{QString{"http://%1/%2"}.arg(params.host, params.publisherPath)}};
    req.setRawHeader(QByteArray{"HOST"}, params.host.toUtf8());
    req.setRawHeader(QByteArray{"CALLBACK"}, params.callback.toUtf8());
    req.setRawHeader(QByteArray{"NT"}, QByteArray{"upnp:event"});
    req.setRawHeader(QByteArray{"TIMEOUT"}, QString{"Second-%1"}.arg(QString::number(params.timeout)).toUtf8());

    mSubscribeRequestPending = mNetworkManager.sendCustomRequest(req, QByteArray{"SUBSCRIBE"});

    connect(mSubscribeRequestPending, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError error) {
        qCCritical(upnpavEvent) << "Event subscription" << mSubscribeRequestPending->url() << "failed. Error:" << error;
    });

    connect(mSubscribeRequestPending, &QNetworkReply::finished, this, [this] {
        if (mSubscribeRequestPending->error() == QNetworkReply::NoError) {
            auto rawTimeout = mSubscribeRequestPending->rawHeader("TIMEOUT");
            auto indexOfDash = rawTimeout.indexOf("-");
            bool conversionOk = false;
            auto timeout =
                static_cast<quint32>(rawTimeout.remove(0, indexOfDash + 1).toFloat(&conversionOk) / 2 * 1000);
            if (not conversionOk) {
                Q_EMIT subscriptionFailed(SubscriptionError::IncompatibleHeader);
                return;
            }
            mSubscriptionRenewTimer.setInterval(std::chrono::milliseconds{timeout});
            mSubscriptionRenewTimer.start();
            setSid(mSubscribeRequestPending->rawHeader(QByteArray{"SID"}));
            setIsSubscribed(true);
            mSubscribeRequestPending->deleteLater();
            mSubscribeRequestPending = nullptr;
        }
    });
    mParams = params;
}

void HttpEventSubscriptionHandle::unsubscribe(EventSubscriptionParameters const& params) noexcept
{
    if (mUnsubscribeRequestPending != nullptr) {
        return;
    }
    auto req = QNetworkRequest{QUrl{QString{"http://%1/%2"}.arg(params.host, params.publisherPath)}};
    req.setRawHeader(QByteArray{"HOST"}, params.host.toUtf8());
    req.setRawHeader(QByteArray{"SID"}, sid().toUtf8());

    mUnsubscribeRequestPending = mNetworkManager.sendCustomRequest(req, QByteArray{"UNSUBSCRIBE"});

    connect(mUnsubscribeRequestPending, &QNetworkReply::finished, mUnsubscribeRequestPending, [this] {
        emitUnsubscribed();
    });
}

void HttpEventSubscriptionHandle::setBody(QString const& body) noexcept
{
    setResponseBody(body);
}

void HttpEventSubscriptionHandleDeleter::operator()(HttpEventSubscriptionHandle* handle)
{
    QObject::connect(handle, &HttpEventSubscriptionHandle::unsubscribed, handle, [handle]() {
        handle->deleteLater();
    });
    handle->unsubscribe(handle->mParams);
}

} // namespace UPnPAV
