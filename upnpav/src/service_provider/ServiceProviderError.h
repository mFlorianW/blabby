// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEPROVIDERERROR_H
#define SERVICEPROVIDERERROR_H

#include "blabbyupnpav_export.h"
#include <QMetaType>
#include <QString>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT ServiceProviderError
{
public:
    enum class ErrorCode : quint8
    {
        WrongDestination = 0, //!< The SSDP message
        EmtpySSDPMessage,
        MalformedSsdpMessage,
        XmlNotWellFormed
    };

    ServiceProviderError();
    ServiceProviderError(ErrorCode errorCode, const QString &errorDescription) noexcept;

    ~ServiceProviderError() noexcept;

    ServiceProviderError(const ServiceProviderError &other);
    ServiceProviderError &operator=(const ServiceProviderError &other);

    ErrorCode errorCode() const;

    QString errorDescription() const;

private:
    ErrorCode m_errorCode;
    QString m_errorDescription;
};

} // namespace UPnPAV

Q_DECLARE_METATYPE(UPnPAV::ServiceProviderError)

#endif // SERVICEPROVIDERERROR_H
