// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include "DeviceDescription.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"
#include "SoapMessageTransmitter.hpp"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>
#include <QUrl>
#include <utility>
namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaDevicePrivate
{
public:
    MediaDevicePrivate(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitter> transmitter)
        : mDeviceDescription{std::move(deviceDescription)}
        , mSoapMessageTransmitter{std::move(transmitter)}
    {
    }

    ~MediaDevicePrivate() = default;

    Q_DISABLE_COPY_MOVE(MediaDevicePrivate)

    DeviceDescription mDeviceDescription;
    ServiceControlPointDefinition mContentDirectorySCPD;
    ServiceDescription mConnectionManagerDescription;
    ServiceControlPointDefinition mConnectionManagerSCPD;
    ServiceDescription mAvTransportDescription;
    ServiceControlPointDefinition mAvTransportDescriptionSCPD;
    QSharedPointer<SoapMessageTransmitter> mSoapMessageTransmitter;
    QString mName{""};
    QUrl mIconUrl{""};
    bool mHasAvTransportService{false};
};

} // namespace UPnPAV
