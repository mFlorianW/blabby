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
#include "ConnectionManagerServiceValidator.h"
#include "ScpdStateVariableValidator.h"
#include "ServiceDescriptionValidator.h"
#include "ScpdActionListValidator.h"

namespace UPnPAV
{

ConnectionManagerServiceValidator::ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription)
{
    m_serviceName = "ConnectionManager";
    m_serviceType = "urn:schemas-upnp-org:service:ConnectionManager";
    m_deviceDescription = deviceDescription;

    m_stateVariables = QVector<QString>
    {
        "SourceProtocolInfo",
        "SinkProtocolInfo",
        "CurrentConnectionIDs",
        "A_ARG_TYPE_ConnectionStatus",
        "A_ARG_TYPE_ConnectionManager",
        "A_ARG_TYPE_Direction",
        "A_ARG_TYPE_ProtocolInfo",
        "A_ARG_TYPE_ConnectionID",
        "A_ARG_TYPE_RcsID"
    };

    m_actions = QVector<QString>
    {
        "GetProtocolInfo",
        "GetCurrentConnectionIDs",
        "GetCurrentConnectionInfo"
    };
}

} //namespace
