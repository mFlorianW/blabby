// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DESCRIPTIONFETCHERBACKEND_H
#define DESCRIPTIONFETCHERBACKEND_H

#include "blabbyupnpav_export.h"
#include <QNetworkAccessManager>
#include <QObject>

class QNetworkReply;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT DescriptionFetcherBackend : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DescriptionFetcherBackend)
public:
    DescriptionFetcherBackend();
    ~DescriptionFetcherBackend() override;

    void fetchDescriptionFrom(QUrl const& url);

Q_SIGNALS:
    void descriptionFetched(QString const& description, QUrl const& url);

protected:
    virtual void fetchDescription(QUrl const& url) = 0;
};

class BLABBYUPNPAV_EXPORT HttpDescriptionFetcherBackend final : public DescriptionFetcherBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HttpDescriptionFetcherBackend)
public:
    HttpDescriptionFetcherBackend();
    ~HttpDescriptionFetcherBackend() override = default;

protected:
    void fetchDescription(QUrl const& url) override;

private Q_SLOTS:
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_sender;
    QVector<QNetworkReply*> m_pendingReplies;
};

} // namespace UPnPAV
#endif // DESCRIPTIONFETCHERBACKEND_H
