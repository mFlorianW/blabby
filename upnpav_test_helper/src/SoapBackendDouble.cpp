// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapBackendDouble.hpp"
#include "TestSoapCall.hpp"

namespace UPnPAV
{

SoapBackendDouble::SoapBackendDouble() = default;

QSharedPointer<SoapCall> SoapBackendDouble::sendSoapMessage(QString const& url,
                                                            QString const& actionName,
                                                            QString const& serviceType,
                                                            QString const& xmlBody) noexcept
{
    Q_UNUSED(url)
    Q_UNUSED(actionName)
    Q_UNUSED(serviceType)
    mXmlMessageBody = xmlBody;

    return QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall()};
}

QSharedPointer<SoapCall> SoapBackendDouble::sendSoapMessage(ServiceDescription const& desc,
                                                            ServiceControlPointDefinition& scpd,
                                                            SCPDAction const& action,
                                                            QString& xmlBody) noexcept
{
    Q_UNUSED(desc)
    Q_UNUSED(scpd)
    Q_UNUSED(action)

    mXmlMessageBody = xmlBody;
    return QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall};
}

QString SoapBackendDouble::xmlMessageBody() const
{
    return mXmlMessageBody;
}

} // namespace UPnPAV
