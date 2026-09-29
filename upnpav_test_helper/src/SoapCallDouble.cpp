// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapCallDouble.hpp"

namespace UPnPAV
{

SoapCallDouble::SoapCallDouble()
    : SoapCallDouble(ServiceControlPointDefinition{}, SCPDAction{})
{
}

SoapCallDouble::SoapCallDouble(ServiceControlPointDefinition scpd, SCPDAction action)
    : SoapCall{std::move(scpd), std::move(action)}
{
}

SoapCallDouble::~SoapCallDouble() = default;

bool SoapCallDouble::hasFinishedSuccesful() const noexcept
{
    return !m_errorState;
}

QString SoapCallDouble::rawMessage() const noexcept
{
    return m_rawMessage;
}

void SoapCallDouble::setRawMessage(QString const& rawMessage)
{
    m_rawMessage = rawMessage;
}

void SoapCallDouble::setErrorState(bool error)
{
    m_errorState = error;
}

void SoapCallDouble::setDeviceUnreachable()
{
    m_errorState = true;
    m_deviceUnreachable = true;
}

bool SoapCallDouble::isDeviceUnreachable() const noexcept
{
    return m_deviceUnreachable;
}

} // namespace UPnPAV
