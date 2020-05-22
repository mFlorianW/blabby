#include "BaseValidator.h"

namespace UPnPAV
{

BaseValidator::~BaseValidator()
{
}

const QString &UPnPAV::BaseValidator::errorMessage() const noexcept
{
    return m_errorMessage;
}

} //namespace
