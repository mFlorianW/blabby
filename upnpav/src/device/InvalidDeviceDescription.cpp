#include "InvalidDeviceDescription.h"
#include <QDebug>

namespace UPnPAV
{

InvalidDeviceDescription::InvalidDeviceDescription(const QString &errorMessage)
    : m_errorMessage(errorMessage.toLocal8Bit())
{
}

const char *InvalidDeviceDescription::what() const noexcept
{
    return m_errorMessage;
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
