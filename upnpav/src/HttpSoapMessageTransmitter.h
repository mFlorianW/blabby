// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef HTTPSOAPMESSAGETRANSMITTER_H
#define HTTPSOAPMESSAGETRANSMITTER_H

#include "SoapMessageTransmitter.h"
#include "blabbyupnpav_export.h"
#include <QNetworkAccessManager>

namespace UPnPAV
{
class BLABBYUPNPAV_EXPORT HttpSoapMessageTransmitter final : public SoapMessageTransmitter
{
public:
    HttpSoapMessageTransmitter();
    ~HttpSoapMessageTransmitter() override;

    Q_DISABLE_COPY_MOVE(HttpSoapMessageTransmitter)

    QSharedPointer<SoapCall> sendSoapMessage(const QString &url,
                                             const QString &actionName,
                                             const QString &serviceType,
                                             const QString &xmlBody) noexcept override;

    QSharedPointer<SoapCall> sendSoapMessage(ServiceDescription const &desc,
                                             ServiceControlPointDefinition &scpd,
                                             SCPDAction const &action,
                                             QString &xmlBody) noexcept override;

private:
    QNetworkAccessManager m_accessManager;
};

} // namespace UPnPAV

#endif // HTTPSOAPMESSAGETRANSMITTER_H
