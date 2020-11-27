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
#include "ServiceProviderDouble.h"
#include <QString>

namespace MediaServerPlugin
{
namespace Doubles
{

void ServiceProviderDouble::setSearchTarget(const QString &searchTarget) noexcept
{
    mSearchTarget = searchTarget;
}

void ServiceProviderDouble::startSearch() const noexcept
{
    const_cast<ServiceProviderDouble *>(this)->Q_EMIT serviceConnected("");
}

UPnPAV::DeviceDescription ServiceProviderDouble::rootDeviceDescription(const QString &usn) const noexcept
{
    Q_UNUSED(usn);
    return UPnPAV::DeviceDescription{};
}

std::unique_ptr<UPnPAV::IServiceProvider> ServiceProviderFactory::createServiceProvider(const QString &searchTarget)
{
    Q_UNUSED(searchTarget);
    return std::make_unique<ServiceProviderDouble>();
}

} // namespace Doubles
} // namespace MediaServerPlugin