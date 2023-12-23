// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDiscoveryPackage.hpp"

namespace UPnPAV
{

ServiceDiscoveryPackage::ServiceDiscoveryPackage(const QByteArray &rawData)
{
    auto rawDataSplitted = QString{rawData}.split("\r\n");

    for (const auto &entry : rawDataSplitted) {
        if (entry.startsWith(QStringLiteral("LOCATION:"))) {
            m_locationUrl = extracEntryValue(QString{entry});
        } else if (entry.startsWith(QStringLiteral("USN:"))) {
            auto deviceUsn = extracEntryValue(QString{entry});
            m_deviceUsn = extracDeviceIdentifierValue(deviceUsn);
        } else if (entry.startsWith(QStringLiteral("NTS:"))) {
            auto nts = extracEntryValue(QString{entry});
            m_notificationSubType = convertSubTypeString(nts);
        } else if (entry.startsWith(QStringLiteral("ST: ")) or entry.startsWith(QStringLiteral("NT: "))) {
            mSearchTarget = extracEntryValue(entry);
        }
    }

    if (m_locationUrl.isEmpty() && m_notificationSubType != SsdpSubType::ByeBye) {
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

SsdpSubType ServiceDiscoveryPackage::notificationSubType() const
{
    return m_notificationSubType;
}

QString const &ServiceDiscoveryPackage::searchTarget() const
{
    return mSearchTarget;
}

QString ServiceDiscoveryPackage::extracEntryValue(const QString &entry)
{
    auto beginOfValue = entry.indexOf(':') + 1;
    if (beginOfValue == 0) {
        return {};
    }

    return entry.mid(beginOfValue).trimmed();
}

QString ServiceDiscoveryPackage::extracDeviceIdentifierValue(const QString &rawString)
{
    if (rawString.contains("::")) {
        return rawString.split("::").at(0).trimmed();
    }

    return rawString.trimmed();
}

SsdpSubType ServiceDiscoveryPackage::convertSubTypeString(const QString &subtype)
{
    if (subtype == QStringLiteral("ssdp:byebye")) {
        return SsdpSubType::ByeBye;
    } else if (subtype == QStringLiteral("ssdp:alive")) {
        return SsdpSubType::Notify;
    }

    return SsdpSubType::Unknown;
}

PackageParseError::PackageParseError(QString description)
    : QException()
    , m_errorDescription(std::move(description).toLocal8Bit())
{
}

const char *PackageParseError::what() const noexcept
{
    return m_errorDescription.data();
}

void PackageParseError::raise() const
{
    throw *this;
}

PackageParseError *PackageParseError::clone() const
{
    return new PackageParseError(*this); // NOLINT cppcoreguidelines-owning-memory
}

} // namespace UPnPAV
