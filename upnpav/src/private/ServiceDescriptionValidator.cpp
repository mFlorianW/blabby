// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDescriptionValidator.hpp"

#include <utility>

namespace UPnPAV
{

ServiceDescriptionValidator::ServiceDescriptionValidator(QString descriptionName, ServiceDescription serviceDescription)
    : m_descriptionName(std::move(descriptionName))
    , m_serviceDescription(std::move(serviceDescription))
{
}

bool ServiceDescriptionValidator::validate() noexcept
{
    if (m_serviceDescription.eventUrl().isEmpty()) {
        m_errorMessage = QString{m_descriptionName + " event URL is not set."};
        return false;
    }

    if (m_serviceDescription.controlUrl().isEmpty()) {
        m_errorMessage = QString{m_descriptionName + " control URL is not set."};
        return false;
    }

    if (m_serviceDescription.id().isEmpty()) {
        m_errorMessage = QString{m_descriptionName + " service ID is not set."};
        return false;
    }

    if (m_serviceDescription.scpdUrl().isEmpty()) {
        m_errorMessage = QString{m_descriptionName + " SCPD URL is not set."};
        return false;
    }

    return true;
}

} // namespace UPnPAV
