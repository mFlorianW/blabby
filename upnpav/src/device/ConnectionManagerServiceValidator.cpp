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
