// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DeviceDescription.hpp"
#include "EventBackend.hpp"
#include "MediaRenderer.hpp"
#include "blabbyupnpav_export.h"
#include <SoapBackend.hpp>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaRendererPrivate
{
public:
    MediaRendererPrivate(DeviceDescription deviceDescription,
                         QSharedPointer<SoapBackend> transmitter,
                         QSharedPointer<EventBackend> eventBackend,
                         MediaRenderer& mediaRenderer)
        : mMediaRenderer{mediaRenderer}
        , mDeviceDescription(std::move(deviceDescription))
        , mSoapMessageTransmitter(std::move(transmitter))
        , mEventBackend(std::move(eventBackend))
    {
    }

    void setMasterVolume(QString rawVolume) noexcept
    {
        bool conversionOk = false;
        auto const volume = rawVolume.toUInt(&conversionOk);
        if (not conversionOk) {
            return;
        }

        if (mMasterVolume != volume) {
            mMasterVolume = volume;
            Q_EMIT mMediaRenderer.masterVolumeChanged(volume);
        }
    }

    void setMasterMute(QString const& rawMute) noexcept
    {
        auto const value = QStringView{rawMute}.trimmed();
        auto const mute = value == u"1" or value.compare(u"true", Qt::CaseInsensitive) == 0 or
                          value.compare(u"yes", Qt::CaseInsensitive) == 0;
        if (mMasterMute != mute) {
            mMasterMute = mute;
            Q_EMIT mMediaRenderer.masterMuteChanged(mute);
        }
    }

    MediaRenderer& mMediaRenderer;
    DeviceDescription mDeviceDescription;
    ServiceDescription mRenderingControlService;
    ServiceControlPointDefinition mRenderingControlSCPD;
    QSharedPointer<SoapBackend> mSoapMessageTransmitter;
    QSharedPointer<EventBackend> mEventBackend;
    std::shared_ptr<EventSubscriptionHandle> mRenderControlEvents;
    quint32 mMasterVolume = 0U;
    std::optional<bool> mMasterMute;
};

} // namespace UPnPAV
