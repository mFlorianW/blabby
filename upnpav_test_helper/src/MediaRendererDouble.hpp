// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "AvTransportActions.hpp"
#include "ConnectionManagerActions.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "EventBackend.hpp"
#include "MediaRenderer.hpp"
#include "SoapCallDouble.hpp"

namespace UPnPAV::Doubles
{

struct AvTransportUriData
{
    quint32 instanceId = quint32{1234};
    QString uri;
    QString uriMetaData;

    friend bool operator==(AvTransportUriData const& lhs, AvTransportUriData const& rhs) = default;
};

struct PlayData
{
    quint32 instanceId = quint32{1234};

    friend bool operator==(PlayData const& lhs, PlayData const& rhs) = default;
};

class MediaRendererDouble : public UPnPAV::MediaRenderer
{
public:
    MediaRendererDouble(DeviceDescription desc,
                        QSharedPointer<SoapBackend> transmitter,
                        QSharedPointer<UPnPAV::EventBackend> eventBackend);
    ~MediaRendererDouble() override;
    Q_DISABLE_COPY_MOVE(MediaRendererDouble)

    std::unique_ptr<PendingSoapCall> protocolInfo() noexcept override;

    QSharedPointer<SoapCallDouble> protocolInfoCall() const noexcept;

    bool isProtocolInfoCalled() const noexcept;

    std::optional<std::unique_ptr<PendingSoapCall>> setAvTransportUri(quint32 instanceId,
                                                                      QString const& uri,
                                                                      QString const& uriMetaData = QString{
                                                                          ""}) noexcept override;

    bool isSetAvTransportUriCalled() const noexcept;

    AvTransportUriData avTransportUriData() const noexcept;

    QSharedPointer<SoapCallDouble> avTransportUriCall() const noexcept;

    std::optional<std::unique_ptr<PendingSoapCall>> play(quint32 instanceId) override;

    bool isPlayCalled() const noexcept;

    PlayData playData() const noexcept;

    QSharedPointer<SoapCallDouble> playCall() const noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::state
     */
    MediaDevice::State state() const noexcept override;

    /**
     * Sets the state for @ref UPnPAV::Doubles::MediaRenderer.
     * @param state The new state for the @ref UPnPAV::Doubles::MediaRenderer.
     */
    void setDeviceState(MediaDevice::State state) noexcept;

private:
    QSharedPointer<SoapCallDouble> mProtoInfoCall =
        QSharedPointer<SoapCallDouble>::create(UPnPAV::validConnectionManagerSCPD(), UPnPAV::GetProtocolInfo());
    bool mIsProtocolInfoCalled = false;
    QSharedPointer<SoapCallDouble> mSetAvTransportUriCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createSetAVTransportURIAction());
    bool mIsSetAvTranstportUriCalled = false;
    AvTransportUriData mSetAvTransportUriData;
    QSharedPointer<SoapCallDouble> mPlayCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createPlayAction());
    bool mIsPlayCalled = false;
    PlayData mPlayData;
    MediaDevice::State mState = MediaDevice::State::NoMediaPresent;
};

} // namespace UPnPAV::Doubles
