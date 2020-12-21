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

#include "IMediaServer.h"

namespace MediaServerPlugin
{
namespace Doubles
{

class MediaServerFactory final : public UPnPAV::IMediaServerFactory
{
public:
    quint32 howOftenCalled{ 0 };
    std::unique_ptr<UPnPAV::IMediaServer> createMediaServer(const UPnPAV::DeviceDescription &deviceDescription) override;
};

class MediaServer : public UPnPAV::IMediaServer
{
public:
    QString name() const noexcept override;

    QUrl iconUrl() const noexcept override;

    QSharedPointer<UPnPAV::PendingSoapCall> getSortCapabilities() noexcept override;

    QSharedPointer<UPnPAV::PendingSoapCall> browse(const QString &objectId, BrowseFlag browseFlag,
                                                   const QString &filter, const QString &sortCriteria) noexcept override;
};

} // namespace Doubles
} // namespace MediaServerPlugin