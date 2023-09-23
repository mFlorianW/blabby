// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapMessageTransmitterDouble.h"
#include "TestSoapCall.h"

namespace UPnPAV
{

SoapMessageTransmitterDouble::SoapMessageTransmitterDouble()
{
}

QSharedPointer<SoapCall> SoapMessageTransmitterDouble::sendSoapMessage(const QString &url,
                                                                       const QString &actionName,
                                                                       const QString &serviceType,
                                                                       const QString &xmlBody) noexcept
{
    Q_UNUSED(url)
    Q_UNUSED(actionName)
    Q_UNUSED(serviceType)
    m_xmlMessageBody = xmlBody;

    return QSharedPointer<TestSoapCall>{new TestSoapCall()};
}

QString SoapMessageTransmitterDouble::xmlMessageBody() const
{
    return m_xmlMessageBody;
}

} // namespace UPnPAV
