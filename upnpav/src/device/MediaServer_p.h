#ifndef MEDIASERVER_P_H
#define MEDIASERVER_P_H

#include "DeviceDescription.h"
#include "ServiceControlPointDefinition.h"
#include "ServiceDescription.h"
#include "SoapMessageTransmitter.h"
#include <QSharedPointer>
#include <QUrl>

namespace UPnPAV
{

class MediaServerPrivate
{
public:
    MediaServerPrivate(const DeviceDescription &deviceDescription, QSharedPointer<SoapMessageTransmitter> transmitter)
        : mDeviceDescription(deviceDescription)
        , mSoapMessageTransmitter(transmitter)
    {
    }

    DeviceDescription mDeviceDescription;
    ServiceDescription mContentDirectoryServiceDescription;
    ServiceControlPointDefinition mContentDirectorySCPD;
    QSharedPointer<SoapMessageTransmitter> mSoapMessageTransmitter;
    QString mName{ "" };
    QUrl mIconUrl{ "" };
};

} // namespace UPnPAV

#endif // MEDIASERVER_P_H
