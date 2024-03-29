// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "EventBackend.hpp"
#include "PendingSoapCall.hpp"
#include "blabbyupnpav_export.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QUrl>
#include <optional>

namespace UPnPAV
{

class MediaDevicePrivate;
class DeviceDescription;
class SoapBackend;

/**
 * This interface defines functions that every upnp media device
 * should support.
 */
class BLABBYUPNPAV_EXPORT MediaDevice : public QObject
{
    Q_OBJECT
public:
    /**
     * Different modes when calling seek on the AVTransport service.
     */
    enum SeekMode
    {
        AbsTime,
        RelTime,
        AbsCount,
        RelCount,
        TrackNr,
        ChannelFreq,
        TapeIndex,
        Frame,
    };
    Q_ENUM(SeekMode)

    /**
     * The different states a MediaDevice can have.
     */
    enum class State
    {
        Stopped,
        PausedPlayback,
        PausedRecording,
        Playing,
        Recording,
        Transitioning,
        NoMediaPresent
    };
    Q_ENUM(State)

    /*
     * Deleted move and copy operations
     */
    Q_DISABLE_COPY_MOVE(MediaDevice)

    /**
     * Default destructor
     */
    ~MediaDevice() override;

    /**
     * Gives the human friendly name of the device.
     *
     * @return The human friendly name of the device.
     */
    virtual QString const& name() const noexcept;

    /**
     * Gives the Url of an icon for the device. The string
     * can be empty when the device doesn't have an icon.
     *
     * @return The a Url to an icon of the device.
     */
    virtual QUrl const& iconUrl() const noexcept;

    /**
     * Gives the current state of the device.
     * The device will be updated with the lastChange event updates of the AVTransport service.
     * If the device has no AVTransport service e.g. MediaServer the state will always be @ref
     * UPnPAV::MediaDevice::State::Idle.
     */
    virtual MediaDevice::State state() const noexcept;

    /**
     * Calls the GetProtocolInfo on the ConnectionManager Interface of the UPnPAV device.
     *
     * @return PendingSoapCall with the result or an error.
     */
    virtual std::unique_ptr<PendingSoapCall> protocolInfo() noexcept;

    /**
     * Calls the GetCurrentConnectionIds on the ConnectionManager of the UPnPAV device.
     * @return PendingSoapCall with the result or an error.
     */
    virtual std::unique_ptr<PendingSoapCall> currentConnectionIds() noexcept;

    /**
     * Calls the GetCurrentConnectionInfo on the ConnectionManager of the UPnPAV device.
     * @return PendingSoapCall with the result or an error.
     */
    virtual std::unique_ptr<PendingSoapCall> currentConnectionInfo(quint32 connectionId) noexcept;

    /**
     * Checks if the media device implements the AVTransport service.
     * @return true The device supports AVTransport service otherwise false.
     */
    bool hasAvTransportService() const noexcept;

    /**
     * Calls the AVTransportURI on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @param uri The URI for the new virtual instance of the AVTransport service.
     * @param uriMetaData The MetaData for the new instance optional. Defaults to empty string.
     * @return PendingSoapCall with the result or an error.
     * The optional will be null if the device doesn't have support for the AVTransport service.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> setAvTransportUri(quint32 instanceId,
                                                                              QString const& uri,
                                                                              QString const& uriMetaData = QString{
                                                                                  ""}) noexcept;

    /**
     * Call the GetMediaInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> mediaInfo(quint32 instanceId);

    /**
     * Calls the GetTransportInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> transportInfo(quint32 instanceId);

    /**
     * Calls the GetPositionInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> positionInfo(quint32 instanceId);

    /**
     * Calls the GetDeviceCapabilities on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> deviceCapilities(quint32 instanceId);

    /**
     * Calls the GetTransportSettings on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> transportSettings(quint32 instanceId);

    /**
     * Calls the Stop on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> stop(quint32 instanceId);

    /**
     * Calls the Play on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The speed option is not supported for the moment.
     * The speed value is always set to 1.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> play(quint32 instanceId);

    /**
     * Calls the Seek on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @param mode Which seek mode shall be used.
     * @param target The target of seek. The value depends on the seek modes.
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The following seek modes need the following target values.
     * RelCount, => need an uint32 as target
     * AbsCount, => need an uint32 as target
     * RelCount, => need an uint32 as target
     * TrackNr, => need an uint32 as target
     * Frame, => need an uint32 as target
     * TapeIndex, => need an uint32 as target
     * AbsTime, => Timestamp in the format h.m.s.z
     * RelTime, => Timestamp in the format h.m.s.z
     * ChannelFreq, => float
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> seek(quint32 instanceId,
                                                                 SeekMode mode,
                                                                 QString const& target);

    /**
     * Calls the Next on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> next(quint32 instanceId);

    /**
     * Calls the Previous on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> previous(quint32 instanceId);

    /**
     * Calls the pause function on the media device.
     * The pause function is an optional function of the AVTransport server.
     * If the device doesn't support the pause function an nullopt will be returned.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies.
     *                   Default 0.
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref UPnPAV::MediaDevice::hasAvTransportService
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> pause(quint32 instanceId = 0) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref UPnPAV::MediaDevice changes it state. E.g. from Stopped to playing
     * State changes only occur when the device has an AVTransport service.
     * Devices without AVTransport service are going to stay in the @ref MediaDevice::State::Stopped.
     */
    void stateChanged();

protected:
    MediaDevice(DeviceDescription deviceDescription,
                QSharedPointer<SoapBackend> soapBackend,
                QSharedPointer<EventBackend> eventBackend);

private:
    QScopedPointer<MediaDevicePrivate> d;
};

} // namespace UPnPAV
