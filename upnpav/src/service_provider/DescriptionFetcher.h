// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DESCRIPTIONFETCHER_H
#define DESCRIPTIONFETCHER_H

#include <QObject>

namespace UPnPAV
{
class DescriptionFetcherBackend;

class DescriptionFetcher : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DescriptionFetcher)
public:
    DescriptionFetcher(DescriptionFetcherBackend *backend);
    ~DescriptionFetcher() override;

    void fetchDescription(const QUrl &url);

Q_SIGNALS:
    void descriptionFetched(const QString &description, const QUrl &url);

private:
    DescriptionFetcherBackend *m_backend;
};

} // namespace UPnPAV

#endif // DESCRIPTIONFETCHER_H
