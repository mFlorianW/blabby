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
#ifndef SERVICEPROVIDERERROR_H
#define SERVICEPROVIDERERROR_H

#include "UPnP_Export.h"
#include <QMetaType>
#include <QString>

namespace UPnPAV
{

class UPNP_EXPORT ServiceProviderError
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