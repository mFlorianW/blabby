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
