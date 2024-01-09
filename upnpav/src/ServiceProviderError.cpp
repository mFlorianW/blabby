// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceProviderError.hpp"
namespace UPnPAV
{

ServiceProviderError& ServiceProviderError::operator=(ServiceProviderError const& other) = default;
ServiceProviderError::ServiceProviderError(ServiceProviderError const& other) = default;

ServiceProviderError::ErrorCode ServiceProviderError::errorCode() const
{
    return m_errorCode;
}

QString ServiceProviderError::errorDescription() const
{
    return m_errorDescription;
}

} // namespace UPnPAV
