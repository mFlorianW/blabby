// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
    ServiceProviderError(ErrorCode errorCode, QString errorDescription) noexcept;

    ~ServiceProviderError() noexcept;

    ServiceProviderError(const ServiceProviderError &other);
    ServiceProviderError &operator=(const ServiceProviderError &other);

    ServiceProviderError(ServiceProviderError &&other) noexcept = default;
    ServiceProviderError &operator=(ServiceProviderError &&other) noexcept = default;

    ErrorCode errorCode() const;

    QString errorDescription() const;

private:
    ErrorCode m_errorCode{ServiceProviderError::ErrorCode::WrongDestination};
    QString m_errorDescription{""};
};

} // namespace UPnPAV

Q_DECLARE_METATYPE(UPnPAV::ServiceProviderError)

#endif // SERVICEPROVIDERERROR_H
