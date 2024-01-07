// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DescriptionFetcherBackend.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace UPnPAV
{

DescriptionFetcherBackend::DescriptionFetcherBackend()
    : QObject()
{
}

DescriptionFetcherBackend::~DescriptionFetcherBackend() = default;

void DescriptionFetcherBackend::fetchDescriptionFrom(QUrl const &url)
{
    fetchDescription(url);
}

HttpDescriptionFetcherBackend::HttpDescriptionFetcherBackend()
{
    (void)connect(&m_sender, &QNetworkAccessManager::finished, this, &HttpDescriptionFetcherBackend::replyFinished);
}

void HttpDescriptionFetcherBackend::fetchDescription(QUrl const &url)
{
    auto reply = m_sender.get(QNetworkRequest{url});
    if (reply == nullptr) {
        return;
    }

    m_pendingReplies.append(reply);
}

void HttpDescriptionFetcherBackend::replyFinished(QNetworkReply *reply)
{
    if (!reply->isFinished() && (reply->error() != QNetworkReply::NoError)) {
        m_pendingReplies.removeAll(reply);
        reply->deleteLater();
        return;
    }

    auto description = QString{reply->readAll()};
    auto url = reply->url();

    reply->deleteLater();
    m_pendingReplies.removeAll(reply);
    Q_EMIT descriptionFetched(description, url);
}

} // namespace UPnPAV
