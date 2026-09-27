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
#include "RenderingControlActions.hpp"
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

struct StopData
{
    quint32 instaneId = quint32{1234};
    friend bool operator==(StopData const& lhs, StopData const& rhs) = default;
};

struct PauseData
{
    quint32 instaneId = quint32{1234};
    friend bool operator==(PauseData const& lhs, PauseData const& rhs) = default;
};

struct VolumeData
{
    quint32 instanceId = quint32{1234};
    QString channel;
    friend bool operator==(VolumeData const& lhs, VolumeData const& rhs) = default;
};

struct SetVolumeData
{
    quint32 instanceId = quint32{1234};
    QString channel;
    quint16 volume;
    friend bool operator==(SetVolumeData const& lhs, SetVolumeData const& rhs) = default;
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
     * Resets all the internal states set all call data to default.
     * @note The state is not touched.
     */
    void reset() noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::state
     */
    MediaDevice::State state() const noexcept override;

    /**
     * Sets the state for @ref UPnPAV::Doubles::MediaRenderer.
     * @param state The new state for the @ref UPnPAV::Doubles::MediaRenderer.
     */
    void setDeviceState(MediaDevice::State state) noexcept;

    /**
     * @return The data of the last stop call.
     */
    StopData stopData() const noexcept;

    /**
     * @return True the Stop was called otherwise false
     */
    bool isStopCalled() const noexcept;

    /**
     * @return Gives the stopCall object e.g. to finish it.
     */
    QSharedPointer<SoapCallDouble> stopCall() const noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::stop
     */
    std::optional<std::unique_ptr<PendingSoapCall>> stop(quint32 instanceId) noexcept override;

    /**
     * @return The data of the last pause call.
     */
    PauseData pauseData() const noexcept;

    /**
     * Activates or disables the pause function.
     * Without enabling the pause function the MediaRenderer behaves like a device without pause function.
     * @param enabled True activates the pause function, false deactivates the pauses function.
     */
    void setPauseEnabled(bool enabled) noexcept;

    /**
     * @return True the pause was called otherwise false
     */
    bool isPauseCalled() const noexcept;

    /**
     * Activates or disables the volume function.
     * Without enabling the volume function of the MediaRenderer behaves like a device without volume function.
     * @param enabled True activates the pause function, false deactivates the pauses function.
     */
    void setVolumeEnabled(bool enabled) noexcept;

    /**
     * @return Gives the pause object e.g. to finish it.
     */
    QSharedPointer<SoapCallDouble> pauseCall() const noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::pause
     */
    std::optional<std::unique_ptr<PendingSoapCall>> pause(quint32 instanceId) noexcept override;

    /**
     * @return The data of the last volume call.
     */
    [[nodiscard]] VolumeData volumeData() const noexcept;

    /**
     * @return True the volume was called otherwise false
     */
    [[nodiscard]] bool isVolumeCalled() const noexcept;

    /**
     * @return Gives the volume call object e.g. to finish it.
     */
    [[nodiscard]] QSharedPointer<SoapCallDouble> volumeCall() const noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::volume
     */
    [[nodiscard]] std::optional<std::unique_ptr<PendingSoapCall>> volume(quint32 instanceId,
                                                                         QString const& channel) noexcept override;

    /**
     * @return The data of the last set volume call.
     */
    [[nodiscard]] SetVolumeData setVolumeData() const noexcept;

    /**
     * @return True the volume was called otherwise false
     */
    [[nodiscard]] bool isSetVolumeCalled() const noexcept;

    /**
     * @return Gives the volume call object e.g. to finish it.
     */
    [[nodiscard]] QSharedPointer<SoapCallDouble> setVolumeCall() const noexcept;

    /**
     * @copydoc UPnPAV::MediaDevice::volume
     */
    [[nodiscard]] std::optional<std::unique_ptr<PendingSoapCall>> setVolume(quint32 instanceId,
                                                                            QString const& channel,
                                                                            quint32 volume) noexcept override;

private:
    // State
    MediaDevice::State mState = MediaDevice::State::NoMediaPresent;
    // ProtocolInfo
    QSharedPointer<SoapCallDouble> mProtoInfoCall =
        QSharedPointer<SoapCallDouble>::create(UPnPAV::validConnectionManagerSCPD(), UPnPAV::GetProtocolInfo());
    bool mIsProtocolInfoCalled = false;
    // AVTransportUri
    QSharedPointer<SoapCallDouble> mSetAvTransportUriCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createSetAVTransportURIAction());
    bool mIsSetAvTranstportUriCalled = false;
    AvTransportUriData mSetAvTransportUriData;
    // Play
    QSharedPointer<SoapCallDouble> mPlayCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createPlayAction());
    bool mIsPlayCalled = false;
    PlayData mPlayData;
    // Stop
    bool mIsStopCalled = false;
    StopData mStopData;
    QSharedPointer<SoapCallDouble> mStopCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createStopAction());
    // Pause
    bool mIsPauseCalled = false;
    bool mPauseEnabled = false;
    PauseData mPauseData;
    QSharedPointer<SoapCallDouble> mPauseCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createPauseAction());
    // Volumue
    bool mIsVolumeCalled = false;
    bool mVolumeEnabled = false;
    VolumeData mVolumeData;
    QSharedPointer<SoapCallDouble> mVolumeCall =
        QSharedPointer<SoapCallDouble>::create(validRenderingControlSCPD(), getVolumeAction());
    // Volumue
    bool mIsSetVolumeCalled = false;
    SetVolumeData mSetVolumeData;
    QSharedPointer<SoapCallDouble> mSetVolumeCall =
        QSharedPointer<SoapCallDouble>::create(validRenderingControlSCPD(), setVolumeAction());
};

} // namespace UPnPAV::Doubles
