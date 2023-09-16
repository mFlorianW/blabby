// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapCallDouble.h"

namespace UPnPAV
{

SoapCallDouble::SoapCallDouble()
{
}

bool SoapCallDouble::hasFinishedSuccesful() const noexcept
{
    return !m_errorState;
}

QString SoapCallDouble::rawMessage() const noexcept
{
    return m_rawMessage;
}

void SoapCallDouble::setRawMessage(const QString &rawMessage)
{
    m_rawMessage = rawMessage;
}

void SoapCallDouble::setErrorState(bool error)
{
    m_errorState = error;
}

} // namespace UPnPAV
