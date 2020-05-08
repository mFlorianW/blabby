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
#ifndef DESCRIPTIONFETCHERBACKEND_H
#define DESCRIPTIONFETCHERBACKEND_H

#include "UPnP_Export.h"

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkReply;

namespace UPnPAV
{

class UPNP_EXPORT DescriptionFetcherBackend : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DescriptionFetcherBackend)
public:
    DescriptionFetcherBackend();
    ~DescriptionFetcherBackend() override;

    void fetchDescriptionFrom(const QUrl &url);

Q_SIGNALS:
    void descriptionFetched(const QString &description, const QUrl &url);

protected:
    virtual void fetchDescription(const QUrl &url) = 0;
};

class UPNP_EXPORT HttpDescriptionFetcherBackend final : public DescriptionFetcherBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HttpDescriptionFetcherBackend)
public:
    HttpDescriptionFetcherBackend();
    ~HttpDescriptionFetcherBackend() = default;

protected:
    void fetchDescription(const QUrl &url) override;

private Q_SLOTS:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_sender;
    QVector<QNetworkReply*> m_pendingReplies;
};

} // namespace UPnPAV
#endif // DESCRIPTIONFETCHERBACKEND_H
