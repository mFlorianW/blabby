#include "SoapCallDouble.h"

namespace MediaServer::Plugin::Doubles
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

} // namespace MediaServer::Plugin::Doubles
