// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDISCOVERYPACKAGE_H
#define SERVICEDISCOVERYPACKAGE_H

#include <QUrl>
#include <QString>
#include <QException>

class QByteArray;
namespace UPnPAV
{
/**
 * The ServiceDiscoveryPackage represents a discovery message. The package
 * can be constructed from a Notify, MSearch or ByeBye message.
 */
class ServiceDiscoveryPackage final
{
public:
    enum SubType
    {
        Unknown = 0, /*!< Unknown sub type*/
        ByeBye, /*!< ssdp:byebye sub type */
        Notify /*!< The package is a ssdp:alive message */
    };

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
    SubType notificationSubType() const;

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
    static SubType convertSubTypeString(const QString &subtype);

private:
    QUrl m_locationUrl;
    QString m_deviceUsn;
    SubType m_notificationSubType{SubType::Unknown};

};

class PackageParseError final : public QException
{
public:
    /**
     * Consturcts the PackageParseError.
     *
     * @param descprition A message that describe the error.
     */
    PackageParseError(const QString &description);
    ~PackageParseError() override;

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
    QString m_errorDescription{""};
};

} //namespace UPnPAV
#endif // SERVICEDISCOVERYPACKAGE_H
