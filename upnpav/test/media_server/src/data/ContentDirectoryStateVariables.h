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
#ifndef CONTENTDIRECTORYSTATEVARIABLES_H
#define CONTENTDIRECTORYSTATEVARIABLES_H

#include "SCPDStateVariable.h"

namespace UPnPAV
{

SCPDStateVariable A_ARG_TYPE_ObjectID
{
    false,
    "A_ARG_TYPE_ObjectID",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_Result
{
    false,
    "A_ARG_TYPE_Result",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_BrowseFlag
{
    false,
    "A_ARG_TYPE_BrowseFlag",
    SCPDStateVariable::DataType::String,
    "",
    {
        "BrowseMetaData"
        "BrowseDirectChildren"
    }
};

SCPDStateVariable A_ARG_TYPE_Filter
{
    false,
    "A_ARG_TYPE_Filter",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_SortCriteria
{
    false,
    "A_ARG_TYPE_SortCriteria",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_Index
{
    false,
    "A_ARG_TYPE_Index",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_Count
{
    false,
    "A_ARG_TYPE_Count",
    SCPDStateVariable::DataType::Ui4
};

SCPDStateVariable A_ARG_TYPE_UpdateID
{
    false,
    "A_ARG_TYPE_UpdateID",
    SCPDStateVariable::DataType::Ui4
};

SCPDStateVariable SearchCapabilities
{
    false,
    "SearchCapabilities",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable SortCapabilities
{
    false,
    "SortCapabilities",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable SystemUpdateID
{
    true,
    "SystemUpdateID",
    SCPDStateVariable::DataType::Ui4
};

} //namespace UPnPAV


#endif // CONTENTDIRECTORYSTATEVARIABLES_H
