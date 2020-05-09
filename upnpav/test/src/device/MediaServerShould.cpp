#include "MediaServerShould.h"
#include "DeviceDescription.h"
#include "MediaServer.h"
#include "InvalidDeviceDescription.h"

#include <QTest>

namespace UPnPAV
{

MediaServerShould::MediaServerShould()
    : QObject()
{
}

void MediaServerShould::beInstantiateableOnlyWithRequiredServiceDescription()
{
    try
    {
        auto serviceDescription = QVector<ServiceDescription>{
                ServiceDescription{QString{"urn:schemas-upnp-org:service:ContentDirectory:1"}, "" , "", "", ""},
                ServiceDescription{QString{"urn:schemas-upnp-org:service:ConnectionManager:1"}, "" , "", "", ""}};
        MediaServer mediaServer{DeviceDescription{"", "", "", "", "",
                                                  QVector<IconDescription>{},
                                                  serviceDescription}};
    }
    catch(InvalidDeviceDescription &e)
    {
        QFAIL("Invalid device description passed.");
    }
}

} // namespace UPnPAV
