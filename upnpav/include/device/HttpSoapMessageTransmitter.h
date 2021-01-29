// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef HTTPSOAPMESSAGETRANSMITTER_H
#define HTTPSOAPMESSAGETRANSMITTER_H

#include "UPnP_Export.h"
#include "SoapMessageTransmitter.h"

#include <QNetworkAccessManager>

namespace UPnPAV
{
class UPNP_EXPORT HttpSoapMessageTransmitter final : public SoapMessageTransmitter
{
public:
    HttpSoapMessageTransmitter();
    ~HttpSoapMessageTransmitter() override;

    QSharedPointer<SoapCall> sendSoapMessage(const QString &url,
                                             const QString &actionName,
                                             const QString &serviceType,
                                             const QString &xmlBody) noexcept override;

private:
    QNetworkAccessManager m_accessManager;
};

} //namespace UPnPAV

#endif // HTTPSOAPMESSAGETRANSMITTER_H
