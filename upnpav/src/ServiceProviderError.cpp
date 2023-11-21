// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceProviderError.hpp"
namespace UPnPAV
{

ServiceProviderError &ServiceProviderError::operator=(const ServiceProviderError &other) = default;
ServiceProviderError::ServiceProviderError(const ServiceProviderError &other) = default;

ServiceProviderError::ErrorCode ServiceProviderError::errorCode() const
{
    return m_errorCode;
}

QString ServiceProviderError::errorDescription() const
{
    return m_errorDescription;
}

} // namespace UPnPAV
