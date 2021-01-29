// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
