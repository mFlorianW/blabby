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
#include "MediaServerDouble.h"
#include "PendingSoapCall.h"
#include <QUrl>

namespace MediaServer::Plugin::Doubles
{

std::unique_ptr<UPnPAV::IMediaServer> MediaServerFactory::createMediaServer(const UPnPAV::DeviceDescription &deviceDescription)
{
    Q_UNUSED(deviceDescription)
    ++howOftenCalled;
    return std::make_unique<MediaServer>();
}

QString MediaServer::name() const noexcept
{
    return "MediaServer";
}

QUrl MediaServer::iconUrl() const noexcept
{
    return QUrl{ "http://localhost:8200/icons/sm.png" };
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    return {};
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::browse(const QString &objectId, BrowseFlag browseFlag,
                                                            const QString &filter, const QString &sortCriteria) noexcept
{
    Q_UNUSED(filter)
    Q_UNUSED(sortCriteria)

    lastBrowseRequest.objectId = objectId;
    lastBrowseRequest.browseFlag = browseFlag;

    return QSharedPointer<UPnPAV::PendingSoapCall>{ new UPnPAV::PendingSoapCall{ soapCall } };
}

} // namespace MediaServer::Plugin::Doubles
