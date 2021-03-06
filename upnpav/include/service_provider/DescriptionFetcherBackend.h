// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
