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
