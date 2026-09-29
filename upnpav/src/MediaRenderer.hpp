// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "MediaDevice.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{
class MediaRendererPrivate;

/**
 * The range of the Volume of a @ref UPnPAV::MediaRenderer.
 */
struct BLABBYUPNPAV_EXPORT VolumeRange
{
    /**
     * The lowest Volume.
     */
    quint32 minimum = 0;

    /**
     * The highest Volume.
     */
    quint32 maximum = 100;

    friend bool operator==(VolumeRange const& lhs, VolumeRange const& rhs) = default;
};

/**
 * The MediaRenderer is facade for UPnPAV MediaRenderer device.
 */
class BLABBYUPNPAV_EXPORT MediaRenderer : public MediaDevice
{
    Q_OBJECT
public:
    /**
     * Creates an instance of the MediaRenderer.
     *
     * @param desc The DeviceDescription of the MediaRenderer.
     * @param msgTransmitter The transmitter for sending SOAP messages to the device.
     *
     * @throws InvalidDeviceDescription when the DeviceDescription doesn't contain the
     *         required functions and state variables.
     */
    MediaRenderer(DeviceDescription desc,
                  QSharedPointer<SoapBackend> msgTransmitter,
                  QSharedPointer<EventBackend> eventBackend);

    //! @cond Doxygen_Suppress
    ~MediaRenderer() override;
    Q_DISABLE_COPY_MOVE(MediaRenderer)
    //! @endcond

    /**
     * Calls the GetVolume function on the rendering control service of the MediaRenderer
     * This an optional function and not every MediaRenderer supports this function.
     * If the function is not supported a std::nullopt is returned.
     *
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies.
     * @param channel The channel which volume level shall be adjusted.
     * @return PendingSoapCall with the result or an error.
     */
    [[nodiscard]] virtual std::optional<std::unique_ptr<PendingSoapCall>> volume(quint32 instanceId,
                                                                                 QString const& channel) noexcept;

    /**
     * Calls the SetVolume function on the rendering control service of the MediaRenderer
     * This an optional function and not every MediaRenderer supports this function.
     * If the function is not supported a std::nullopt is returned.
     *
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies.
     * @param channel The channel that volume shall be adjusted.
     * @param volume The volume level for the channel.
     * @return PendingSoapCall with the result or an error.
     */
    [[nodiscard]] virtual std::optional<std::unique_ptr<PendingSoapCall>> setVolume(quint32 instanceId,
                                                                                    QString const& channel,
                                                                                    quint32 volume) noexcept;

    /**
     * Gives the range of the Volume from the allowed value range of the Volume state variable of the rendering control
     * service, 0 to 100 when the service defines no range.
     * @return The range of the Volume.
     */
    [[nodiscard]] virtual VolumeRange volumeRange() const noexcept;

    /**
     * Gives whether the rendering control service offers SetVolume.
     * @return True when @ref UPnPAV::MediaRenderer::setVolume can be called, otherwise false.
     */
    [[nodiscard]] virtual bool canSetVolume() const noexcept;

    /**
     * Calls the GetMute function on the rendering control service of the MediaRenderer
     * This an optional function and not every MediaRenderer supports this function.
     * If the function is not supported a std::nullopt is returned.
     *
     * @param instanceId Identifies the virtual instance of the rendering control service to which the action applies.
     * @param channel The channel whose Mute is requested.
     * @return PendingSoapCall with the result or an error.
     */
    [[nodiscard]] virtual std::optional<std::unique_ptr<PendingSoapCall>> mute(quint32 instanceId,
                                                                               QString const& channel) noexcept;

    /**
     * Calls the SetMute function on the rendering control service of the MediaRenderer
     * This an optional function and not every MediaRenderer supports this function.
     * If the function is not supported a std::nullopt is returned.
     *
     * @param instanceId Identifies the virtual instance of the rendering control service to which the action applies.
     * @param channel The channel that shall be muted or unmuted.
     * @param mute True mutes the channel, false unmutes it.
     * @return PendingSoapCall with the result or an error.
     */
    [[nodiscard]] virtual std::optional<std::unique_ptr<PendingSoapCall>> setMute(quint32 instanceId,
                                                                                  QString const& channel,
                                                                                  bool mute) noexcept;

    /**
     * Gives whether the rendering control service offers SetMute.
     * @return True when @ref UPnPAV::MediaRenderer::setMute can be called, otherwise false.
     */
    [[nodiscard]] virtual bool canSetMute() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the master volume is changed.
     */
    void masterVolumeChanged(quint32 volume);

    /**
     * This signal is emitted when the Mute of the master channel is changed.
     */
    void masterMuteChanged(bool mute);

private:
    std::unique_ptr<MediaRendererPrivate> d;
};

/**
 * MediaRenderer simplifies the creation of the MediaRenderer and makes it possible
 * to replace real devices with e.g. test dummies.
 */
class BLABBYUPNPAV_EXPORT MediaRendererFactory
{
public:
    /**
     * Default constructor
     */
    MediaRendererFactory() = default;

    /**
     * Default virtual destructor
     */
    virtual ~MediaRendererFactory() = default;

    /**
     * Copy constructor
     */
    MediaRendererFactory(MediaRendererFactory const& other) = default;

    /**
     * Copy operator
     */
    MediaRendererFactory& operator=(MediaRendererFactory const& other) = default;

    /**
     * Move constructor
     */
    MediaRendererFactory(MediaRendererFactory&& other) = default;

    /**
     * Move operator
     */
    MediaRendererFactory& operator=(MediaRendererFactory&& other) = default;

    /**
     * Factory method that creates tries to create a @ref UPnPAV::MediaRenderer instance.
     * @throws InvalidDeviceDescription when the DeviceDescription doesn't contain the
     *         required functions and state variables.
     * @return The freestore allocated @ref UPnPAV::MediaRenderer
     */
    virtual std::unique_ptr<MediaRenderer> create(DeviceDescription const& desc);
};

} // namespace UPnPAV
