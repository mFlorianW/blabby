/**
 ** This file is part of the Blabby project.
 ** Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
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
#ifndef SOAPMESSAGETRANSMITTER_H
#define SOAPMESSAGETRANSMITTER_H

#include "UPnP_Export.h"
#include "SoapCall.h"

#include <QSharedPointer>

class QString;

namespace UPnPAV
{

class UPNP_EXPORT SoapMessageTransmitter
{
public:
    virtual ~SoapMessageTransmitter();

    virtual QSharedPointer<SoapCall> sendSoapMessage(const QString &url,
                                                     const QString &actionName,
                                                     const QString &serviceType,
                                                     const QString &xmlBody) noexcept = 0;
};

} //namespace UPnPAV

#endif // SOAPTRANSMITTER_H
