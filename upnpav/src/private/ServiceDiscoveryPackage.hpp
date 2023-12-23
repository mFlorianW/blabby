// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDISCOVERYPACKAGE_H
#define SERVICEDISCOVERYPACKAGE_H

#include "blabbyupnpav_export.h"
#include <QException>
#include <QString>
#include <QUrl>

class QByteArray;
namespace UPnPAV
{

Q_NAMESPACE
/**
 * Package Subtypes
 */
enum class SsdpSubType
{
    Unknown = 0, /*!< Unknown sub type e.g. search request response */
    ByeBye, /*!< ssdp:byebye sub type */
    Notify /*!< The package is a ssdp:alive message */
};
Q_ENUM_NS(SsdpSubType)

/**
 * The ServiceDiscoveryPackage represents a discovery message. The package
 * can be constructed from a Notify, MSearch or ByeBye message.
 */
class BLABBYUPNPAV_EXPORT ServiceDiscoveryPackage final
{
public:
    /**
     * Constructs ServiceDiscoveryPackage and extracts the information from it.
     *
     * @param rawData The raw data of the message.
     */
    ServiceDiscoveryPackage(const QByteArray &rawData);

    /**
     * Gives the location URL. If the package is not a ByeBye message
     * then the location url will be empty.
     *
     * @return The location URL of the message.
     */
    QUrl locationUrl() const;

    /**
     * Gives the unique_identifier of the device. This UUID id part of
     * the USN entry in message.
     *
     * @return The unique_identifer of the device.
     */
    QString deviceId() const;

    /**
     * Gives the message NTS of the message.
     *
     * @return The NTS of the message.
     */
    SsdpSubType notificationSubType() const;

    /**
     * Gives the search target of the @ref UPnPAV::ServiceDiscoveryPackage message.
     * @return The search target of the @ref UPnPAV::ServiceDiscoveryPackage message.
     */
    QString const &searchTarget() const;

private:
    /**
     * Extracs the value of a entry in the NOTIFY message. The value
     * is the part after the first colon in the message. E.g.
     * raw ST: <SEARCH Target> will return <SEARCH Target>. If the string
     * doesn't contain any value an empty string will be returned.
     *
     * @param entry The raw entry in form of key:value.
     * @return The valid value or invalid optional in the case no value
     *         found.
     */
    static QString extracEntryValue(const QString &entry);

    /**
     * Extras the UUID part of the USN entry and this is the unique identifier
     * of the device.
     *
     * @param rawString The USN raw string.
     * @return The UUID part of the USN.
     */
    static QString extracDeviceIdentifierValue(const QString &rawString);

    /**
     * Converts the NTS from the message to the enum Type SubType.
     *
     * @param subtype The subtype string from the message.
     * @return The convertion of the message NTS to the enum value SubType.
     */
    static SsdpSubType convertSubTypeString(const QString &subtype);

private:
    QUrl m_locationUrl;
    QString m_deviceUsn;
    SsdpSubType m_notificationSubType{SsdpSubType::Unknown};
    QString mSearchTarget;
};

class PackageParseError final : public QException
{
public:
    /**
     * Consturcts the PackageParseError.
     *
     * @param descprition A message that describe the error.
     */
    PackageParseError(QString description);

    /**
     * Gives the description of the error. The pointer
     * is only valid as long the object exists.
     *
     * @return The detailed description of the error.
     */
    const char *what() const noexcept override;

    /**
     * Raise a new exception.
     */
    void raise() const override;

    /**
     * Creates a new instance of the exception.
     * @return A cloned instance of the exception.
     */
    PackageParseError *clone() const override;

private:
    QByteArray m_errorDescription{""};
};

} // namespace UPnPAV
#endif // SERVICEDISCOVERYPACKAGE_H
