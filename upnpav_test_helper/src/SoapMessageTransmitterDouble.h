// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
