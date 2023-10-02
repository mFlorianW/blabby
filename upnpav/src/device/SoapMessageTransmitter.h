// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPMESSAGETRANSMITTER_H
#define SOAPMESSAGETRANSMITTER_H

#include "ServiceControlPointDefinition.h"
#include "ServiceDescription.h"
#include "SoapCall.h"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>

class QString;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT SoapMessageTransmitter
{
public:
    virtual ~SoapMessageTransmitter();

    Q_DISABLE_COPY_MOVE(SoapMessageTransmitter);

    virtual QSharedPointer<SoapCall> sendSoapMessage(const QString &url,
                                                     const QString &actionName,
                                                     const QString &serviceType,
                                                     const QString &xmlBody) noexcept = 0;

    virtual QSharedPointer<SoapCall> sendSoapMessage(ServiceDescription const &desc,
                                                     ServiceControlPointDefinition &scpd,
                                                     SCPDAction const &action,
                                                     QString &xmlBody) noexcept;

protected:
    SoapMessageTransmitter() = default;
};

} // namespace UPnPAV

#endif // SOAPTRANSMITTER_H
