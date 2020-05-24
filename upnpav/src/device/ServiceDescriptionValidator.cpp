/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "ServiceDescriptionValidator.h"

namespace UPnPAV
{


ServiceDescriptionValidator::ServiceDescriptionValidator(const QString descriptionName,
                                                         const ServiceDescription &serviceDescription)
    : m_descriptionName(descriptionName)
    , m_serviceDescription(serviceDescription)
{
}

bool ServiceDescriptionValidator::validate() noexcept
{
    if(m_serviceDescription.eventUrl().isEmpty())
    {
        m_errorMessage = QString{m_descriptionName + " event URL is not set."};
        return false;
    }

    if(m_serviceDescription.controlUrl().isEmpty())
    {
        m_errorMessage = QString{m_descriptionName + " control URL is not set."};
        return false;
    }

    if(m_serviceDescription.id().isEmpty())
    {
        m_errorMessage = QString{m_descriptionName + " service ID is not set."};
        return false;
    }

    if(m_serviceDescription.scpdUrl().isEmpty())
    {
        m_errorMessage = QString{m_descriptionName + " SCPD URL is not set."};
        return false;
    }

    return true;
}

} //namespace UPnPAV
