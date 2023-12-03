// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapMessageTransmitterDouble.hpp"
#include "TestSoapCall.hpp"

namespace UPnPAV
{

SoapMessageTransmitterDouble::SoapMessageTransmitterDouble() = default;

QSharedPointer<SoapCall> SoapMessageTransmitterDouble::sendSoapMessage(const QString &url,
                                                                       const QString &actionName,
                                                                       const QString &serviceType,
                                                                       const QString &xmlBody) noexcept
{
    Q_UNUSED(url)
    Q_UNUSED(actionName)
    Q_UNUSED(serviceType)
    mXmlMessageBody = xmlBody;

    return QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall()};
}

QSharedPointer<SoapCall> SoapMessageTransmitterDouble::sendSoapMessage(ServiceDescription const &desc,
                                                                       ServiceControlPointDefinition &scpd,
                                                                       SCPDAction const &action,
                                                                       QString &xmlBody) noexcept
{
    Q_UNUSED(desc)
    Q_UNUSED(scpd)
    Q_UNUSED(action)

    mXmlMessageBody = xmlBody;
    return QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall};
}

QString SoapMessageTransmitterDouble::xmlMessageBody() const
{
    return mXmlMessageBody;
}

} // namespace UPnPAV
