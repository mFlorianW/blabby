// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapMessageTransmitter.hpp"

namespace UPnPAV
{

SoapMessageTransmitter::~SoapMessageTransmitter() = default;

QSharedPointer<SoapCall> SoapMessageTransmitter::sendSoapMessage(ServiceDescription const &desc,
                                                                 ServiceControlPointDefinition &scpd,
                                                                 SCPDAction const &action,
                                                                 QString &xmlBody) noexcept
{
    Q_UNUSED(desc)
    Q_UNUSED(scpd)
    Q_UNUSED(action)
    Q_UNUSED(xmlBody)
    return QSharedPointer<SoapCall>{nullptr};
}

} // namespace UPnPAV
