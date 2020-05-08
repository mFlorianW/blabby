/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "DescriptionFetcherBackend.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace UPnPAV
{

DescriptionFetcherBackend::DescriptionFetcherBackend()
    : QObject()
{
}

DescriptionFetcherBackend::~DescriptionFetcherBackend()
{
}

void DescriptionFetcherBackend::fetchDescriptionFrom(const QUrl &url)
{
    fetchDescription(url);
}

HttpDescriptionFetcherBackend::HttpDescriptionFetcherBackend()
{
    (void)connect(&m_sender,
                  &QNetworkAccessManager::finished,
                  this,
                  &HttpDescriptionFetcherBackend::replyFinished);
}

void HttpDescriptionFetcherBackend::fetchDescription(const QUrl &url)
{
    auto reply = m_sender.get(QNetworkRequest{url});
    if(reply == nullptr)
    {
        return;
    }

    m_pendingReplies.append(reply);
}

void HttpDescriptionFetcherBackend::replyFinished(QNetworkReply *reply)
{
    if(!reply->isFinished() && (reply->error() != QNetworkReply::NoError))
    {
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
