#include "AvTransportServiceValidator.h"

namespace UPnPAV
{

AvTransportServiceValidator::AvTransportServiceValidator(DeviceDescription &deviceDescription)
{
    m_serviceName = QStringLiteral("AVTransport");
    m_serviceType = QStringLiteral("urn:schemas-upnp-org:service:AVTransport");
    m_deviceDescription = deviceDescription;

    m_stateVariables = QVector<QString>{};

    m_actions = QVector<QString>{};
}
} // namespace UPnPAV
