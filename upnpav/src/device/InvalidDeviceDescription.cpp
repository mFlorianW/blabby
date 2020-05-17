#include "InvalidDeviceDescription.h"

namespace UPnPAV
{

InvalidDeviceDescription::InvalidDeviceDescription(const QString &errorMessage)
    : m_errorMessage(errorMessage)
{
}

const char *InvalidDeviceDescription::what() const noexcept
{
    return m_errorMessage.toUtf8().data();
}

void InvalidDeviceDescription::raise() const
{
    throw *this;
}

InvalidDeviceDescription *InvalidDeviceDescription::clone() const
{
    return new InvalidDeviceDescription(*this);
}

} //namespace UPnPAV
