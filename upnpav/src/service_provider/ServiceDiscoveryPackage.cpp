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
#include "ServiceDiscoveryPackage.h"

namespace UPnPAV
{

ServiceDiscoveryPackage::ServiceDiscoveryPackage(const QByteArray &rawData)
{
    auto rawDataSplitted = QString{rawData}.split("\r\n");

    for(const auto &entry : rawDataSplitted)
    {
        if(entry.contains(QStringLiteral("LOCATION:")))
        {
            m_locationUrl = extracEntryValue(QString{entry});
        }
        else if(entry.contains(QStringLiteral("USN:")))
        {
            auto deviceUsn = extracEntryValue(QString{entry});
            m_deviceUsn = extracDeviceIdentifierValue(deviceUsn);
        }
        else if(entry.contains(QStringLiteral("NTS:")))
        {
            auto nts = extracEntryValue(QString{entry});
            m_notificationSubType = convertSubTypeString(nts);
        }
    }

    if(m_locationUrl.isEmpty() && m_notificationSubType != SubType::ByeBye)
    {
        throw PackageParseError(QStringLiteral("The package is missing the location URL"));
    }
}

QUrl UPnPAV::ServiceDiscoveryPackage::locationUrl() const
{
    return m_locationUrl;
}

QString UPnPAV::ServiceDiscoveryPackage::deviceId() const
{
    return m_deviceUsn;
}

ServiceDiscoveryPackage::SubType ServiceDiscoveryPackage::notificationSubType() const
{
    return m_notificationSubType;
}

QString ServiceDiscoveryPackage::extracEntryValue(const QString &entry)
{
    auto beginOfValue = entry.indexOf(':') + 1;
    if(beginOfValue == 0)
    {
        return {};
    }

    return entry.mid(beginOfValue).trimmed();
}

QString ServiceDiscoveryPackage::extracDeviceIdentifierValue(const QString &rawString)
{
    if(rawString.contains("::"))
    {
        return rawString.split("::").at(0).trimmed();
    }

    return rawString.trimmed();
}

ServiceDiscoveryPackage::SubType ServiceDiscoveryPackage::convertSubTypeString(const QString &subtype)
{
    if(subtype == QStringLiteral("ssdp:byebye"))
    {
        return SubType::ByeBye;
    }
    else if(subtype == QStringLiteral("ssdp:alive"))
    {
        return SubType::Notify;
    }

    return Unknown;
}

PackageParseError::PackageParseError(const QString &description)
    : QException()
    , m_errorDescription(description)
{

}

PackageParseError::~PackageParseError()
{
}

const char *PackageParseError::what() const noexcept
{
    return m_errorDescription.toLocal8Bit().data();
}

void PackageParseError::raise() const
{
    throw *this;
}

PackageParseError *PackageParseError::clone() const
{
    return new PackageParseError(*this);
}

} //namespace UPnPAV
