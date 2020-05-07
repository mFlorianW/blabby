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
#include "DescriptionFetcher.h"
#include "DescriptionFetcherBackend.h"

namespace UPnPAV
{


DescriptionFetcher::DescriptionFetcher(const QSharedPointer<DescriptionFetcherBackend> backend)
    : QObject()
    , m_backend(backend)
{
    (void)connect(m_backend.get(),
                  &DescriptionFetcherBackend::descriptionFetched,
                  this,
                  &DescriptionFetcher::descriptionFetched);
}

DescriptionFetcher::~DescriptionFetcher() = default;

void DescriptionFetcher::fetchDescription(const QUrl &url)
{
    m_backend->fetchDescriptionFrom(url);
}

} // namespace UPnPAV
