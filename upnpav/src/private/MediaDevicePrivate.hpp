// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include "DeviceDescription.hpp"
#include "EventBackend.hpp"
#include "MediaDevice.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"
#include "SoapBackend.hpp"
#include "blabbyupnpav_export.h"
#include "private/LoggingCategories.hpp"
#include <QSharedPointer>
#include <QUrl>
#include <utility>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaDevicePrivate
{
public:
    MediaDevicePrivate(DeviceDescription deviceDescription,
                       QSharedPointer<SoapBackend> transmitter,
                       QSharedPointer<EventBackend> eventBackend,
                       MediaDevice& device)
        : q{device}
        , mDeviceDescription{std::move(deviceDescription)}
        , mSoapMessageTransmitter{std::move(transmitter)}
        , mEventBackend{std::move(eventBackend)}
    {
    }

    ~MediaDevicePrivate() = default;

    Q_DISABLE_COPY_MOVE(MediaDevicePrivate)

    void setState(MediaDevice::State const state) noexcept
    {
        if (mState != state) {
            mState = state;
            qCDebug(upnpavEvent) << "Device State:" << mState;
            Q_EMIT q.stateChanged();
        }
    }

    MediaDevice& q;

    DeviceDescription mDeviceDescription;
    ServiceControlPointDefinition mContentDirectorySCPD;
    ServiceDescription mConnectionManagerDescription;
    ServiceControlPointDefinition mConnectionManagerSCPD;
    ServiceDescription mAvTransportDescription;
    ServiceControlPointDefinition mAvTransportDescriptionSCPD;
    QSharedPointer<SoapBackend> mSoapMessageTransmitter;
    QString mName{""};
    QUrl mIconUrl{""};
    bool mHasAvTransportService{false};
    QSharedPointer<EventBackend> mEventBackend;
    std::shared_ptr<EventSubscriptionHandle> mAvTransportEvents;
    MediaDevice::State mState = MediaDevice::State::NoMediaPresent;
};

} // namespace UPnPAV
