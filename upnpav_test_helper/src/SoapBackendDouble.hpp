// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPMESSAGETRANSMITTERDOUBLE_H
#define SOAPMESSAGETRANSMITTERDOUBLE_H

#include "SoapBackend.hpp"
#include <QString>

namespace UPnPAV
{

class SoapBackendDouble : public SoapBackend
{
public:
    SoapBackendDouble();

    QSharedPointer<SoapCall> sendSoapMessage(QString const& url,
                                             QString const& actionName,
                                             QString const& serviceType,
                                             QString const& xmlBody) noexcept override;

    QSharedPointer<SoapCall> sendSoapMessage(ServiceDescription const& desc,
                                             ServiceControlPointDefinition& scpd,
                                             SCPDAction const& action,
                                             QString& xmlBody) noexcept override;

    QString xmlMessageBody() const;

    /**
     * Gives the last call that was sent. Like with the real SOAP backend, only a call sent with its SCPD and action
     * knows them, which is needed to read its answer.
     */
    QSharedPointer<SoapCall> const& lastCall() const noexcept;

private:
    QString mXmlMessageBody;
    QSharedPointer<SoapCall> mLastCall;
};

} // namespace UPnPAV

#endif // SOAPMESSAGETRANSMITTERDOUBLE_H
