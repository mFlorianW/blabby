// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
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

    mLastCall = QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall()};
    return mLastCall;
}

QSharedPointer<SoapCall> SoapBackendDouble::sendSoapMessage(ServiceDescription const& desc,
                                                            ServiceControlPointDefinition& scpd,
                                                            SCPDAction const& action,
                                                            QString& xmlBody) noexcept
{
    Q_UNUSED(desc)

    mXmlMessageBody = xmlBody;
    mLastCall = QSharedPointer<TestSoapCall>{new (std::nothrow) TestSoapCall{scpd, action}};
    return mLastCall;
}

QSharedPointer<SoapCall> const& SoapBackendDouble::lastCall() const noexcept
{
    return mLastCall;
}

QString SoapBackendDouble::xmlMessageBody() const
{
    return mXmlMessageBody;
}

} // namespace UPnPAV
