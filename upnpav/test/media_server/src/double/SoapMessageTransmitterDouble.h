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
#ifndef SOAPMESSAGETRANSMITTERDOUBLE_H
#define SOAPMESSAGETRANSMITTERDOUBLE_H

#include "SoapMessageTransmitter.h"

#include <QString>

namespace UPnPAV
{

class SoapMessageTransmitterDouble : public SoapMessageTransmitter
{
public:
    SoapMessageTransmitterDouble();

    QSharedPointer<SoapCall> sendSoapMessage(const QString &url,
                                             const QString &actionName,
                                             const QString &serviceType,
                                             const QString &xmlBody) noexcept override;

    QString xmlMessageBody() const;

private:
    QString m_xmlMessageBody;

};

} //namespace UPnPAV

#endif // SOAPMESSAGETRANSMITTERDOUBLE_H
