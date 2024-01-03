// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPMESSAGETRANSMITTERDOUBLE_H
#define SOAPMESSAGETRANSMITTERDOUBLE_H

#include "SoapMessageTransmitter.hpp"
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

    QSharedPointer<SoapCall> sendSoapMessage(ServiceDescription const &desc,
                                             ServiceControlPointDefinition &scpd,
                                             SCPDAction const &action,
                                             QString &xmlBody) noexcept override;

    QString xmlMessageBody() const;

private:
    QString mXmlMessageBody;
};

} // namespace UPnPAV

#endif // SOAPMESSAGETRANSMITTERDOUBLE_H
