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
        setSid(mSubscribeRequestPending->rawHeader(QByteArray{"SID"}));
        setIsSubscribed(true);
        mSubscribeRequestPending->deleteLater();
        mSubscribeRequestPending = nullptr;
    });
}

} // namespace UPnPAV
