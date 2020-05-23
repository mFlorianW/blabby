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
