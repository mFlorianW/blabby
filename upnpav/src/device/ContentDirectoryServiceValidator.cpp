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
#include "ContentDirectoryServiceValidator.h"
#include "ScpdStateVariableValidator.h"

namespace UPnPAV
{

ContentDirectoryServiceValidator::ContentDirectoryServiceValidator(const DeviceDescription &deviceDescription)
{
    m_serviceName = "ContentDirectory";
    m_serviceType = "urn:schemas-upnp-org:service:ContentDirectory";
    m_deviceDescription = deviceDescription;

    m_stateVariables = QVector<QString>
    {
        "A_ARG_TYPE_ObjectID",
        "A_ARG_TYPE_Result",
        "A_ARG_TYPE_BrowseFlag",
        "A_ARG_TYPE_Filter",
        "A_ARG_TYPE_SortCriteria",
        "A_ARG_TYPE_Index",
        "A_ARG_TYPE_Count",
        "A_ARG_TYPE_UpdateID",
        "SearchCapabilities",
        "SortCapabilities",
        "SystemUpdateID",
    };

    m_actions = QVector<QString>
    {
        "GetSearchCapabilities",
        "GetSortCapabilities",
        "GetSystemUpdateID",
        "Browse"
    };
}

} //namespace UPnPAV
