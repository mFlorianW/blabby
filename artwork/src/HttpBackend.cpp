// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpBackend.hpp"
#include <QNetworkReply>
#include <QPointer>

namespace Artwork
{

HttpReply::HttpReply() = default;
HttpReply::~HttpReply() = default;

int HttpReply::status() const noexcept
{
    return mStatus;
}

QByteArray const& HttpReply::body() const noexcept
{
    return mBody;
}

void HttpReply::finish(int status, QByteArray body)
{
    mStatus = status;
    mBody = std::move(body);
    Q_EMIT finished();
}

HttpBackend::~HttpBackend() = default;

namespace
{

/**
 * The reply of the @ref NetworkHttpBackend, it aborts its request when it's destroyed before the request finished.
 */
class NetworkHttpReply final : public HttpReply
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(NetworkHttpReply)
public:
    explicit NetworkHttpReply(QNetworkReply* reply)
        : mReply{reply}
    {
        (void)connect(mReply, &QNetworkReply::finished, this, [this] {
            auto const status = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            finish(status, status != 0 ? mReply->readAll() : QByteArray{});
        });
    }

    ~NetworkHttpReply() override
    {
        if (mReply != nullptr and mReply->isRunning()) {
            mReply->disconnect(this);
            mReply->abort();
        }
    }

private:
    QPointer<QNetworkReply> mReply;
};

} // namespace

NetworkHttpBackend::NetworkHttpBackend()
{
    mNetworkAccess.setAutoDeleteReplies(true);
}

NetworkHttpBackend::~NetworkHttpBackend() = default;

std::unique_ptr<HttpReply> NetworkHttpBackend::get(QNetworkRequest const& request)
{
    return std::make_unique<NetworkHttpReply>(mNetworkAccess.get(request));
}

} // namespace Artwork

#include "HttpBackend.moc"
