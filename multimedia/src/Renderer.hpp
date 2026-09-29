// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Clock.hpp"
#include "Item.hpp"
#include "MediaRenderer.hpp"
#include "RendererStore.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>

namespace Multimedia
{

/**
 * The Current Track of a @ref Multimedia::Renderer: the Playable it is playing, paused or stopped on, as reported by
 * the Renderer, no matter which controller started it. Details the Renderer doesn't report are empty.
 */
struct BLABBYMULTIMEDIA_EXPORT CurrentTrack
{
    /**
     * The title, the file name of the track without its extension when the Renderer reports no title.
     */
    QString title;

    /**
     * The artist, the creator when the Renderer reports no artist.
     */
    QString artist;

    /**
     * The album.
     */
    QString album;

    /**
     * The year, taken from the date.
     */
    QString year;

    /**
     * The URL of the artwork.
     */
    QString artworkUrl;

    /**
     * The format, the short name of the MIME type plus bit depth and sample rate when known,
     * e.g. "FLAC · 24-bit / 96 kHz". Empty for an unknown MIME type.
     */
    QString format;

    friend bool operator==(CurrentTrack const& lhs, CurrentTrack const& rhs) = default;
};

/**
 * A @ref Mulitmedia::MediaRenderer represents a hardware device
 * that can control and play @ref Mulitmedia::MediaItem
 */
class BLABBYMULTIMEDIA_EXPORT Renderer : public QObject
{
    Q_OBJECT
public:
    /**
     * The state a @ref Multimedia::Renderer can have.
     */
    enum class State
    {
        /**
         * The default state of the Renderer or when the UPnPAV has no media set.
         */
        NoMedia,
        /**
         * The state when the @ref Mulitmedia::Renderer is not active playing
         */
        Stopped,
        /**
         * This is the state when the @ref Mulitmedia::MediaRenderer is active playing a @ref Mulitmedia::Item.
         */
        Playing,
        /**
         * This is state when the @ref Mulitmedia::MediaRenderer active playing is paused.
         */
        Paused,
    };
    Q_ENUM(State)

    /**
     * Whether a @ref Multimedia::Renderer is currently reachable on the network.
     */
    enum class Availability
    {
        /**
         * The @ref Multimedia::Renderer is reachable and can play.
         */
        Online,
        /**
         * The @ref Multimedia::Renderer is not reachable, only its last known details are known.
         */
        Offline,
    };
    Q_ENUM(Availability)

    /**
     * Creates an Online instance of the Renderer
     * @param mediaRenderer The @ref UPnPAV::MediaRenderer that shall be controlled by this @ref Multimedia::Renderer.
     * @param clock The clock that drives the polling of the position.
     */
    Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer,
             std::unique_ptr<UPnPAV::Clock> clock = std::make_unique<UPnPAV::SteadyClock>());

    /**
     * Creates an Offline instance of a remembered Renderer
     * @param remembered The last known details of the @ref Multimedia::Renderer.
     * @param clock The clock that drives the polling of the position.
     */
    explicit Renderer(RememberedRenderer remembered,
                      std::unique_ptr<UPnPAV::Clock> clock = std::make_unique<UPnPAV::SteadyClock>());

    /**
     * Default destructor
     */
    ~Renderer() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(Renderer)

    /**
     * Gives the name of the @ref Mulitmedia::Renderer
     * @return The name of the @ref Mulitmedia::Renderer.
     */
    QString const& name() const noexcept;

    /**
     * Gives the icon of the @ref Mulitmedia::Renderer
     * @return The icon url of the @ref Multimedia::Renderer.
     */
    QString iconUrl() const noexcept;

    /**
     * Gives the identity of the @ref Multimedia::Renderer, the unique device name (UDN) of the device.
     * @return The identity of the @ref Multimedia::Renderer.
     */
    QString const& identity() const noexcept;

    /**
     * Gives the manufacturer of the @ref Multimedia::Renderer.
     * @return The manufacturer or an empty string when the device doesn't name one.
     */
    QString const& manufacturer() const noexcept;

    /**
     * Gives the model name of the @ref Multimedia::Renderer.
     * @return The model name or an empty string when the device doesn't name one.
     */
    QString const& modelName() const noexcept;

    /**
     * Gives the network address of the @ref Multimedia::Renderer.
     * @return The address or an empty string when the address is unknown.
     */
    QString const& address() const noexcept;

    /**
     * Gives the details of the @ref Multimedia::Renderer that are remembered across restarts.
     * @return The identity and the last known name, manufacturer, model name and address.
     */
    RememberedRenderer const& remembered() const noexcept;

    /**
     * Gives the Availability of the @ref Multimedia::Renderer.
     * @return Online when the @ref Multimedia::Renderer is reachable, otherwise Offline.
     */
    Renderer::Availability availability() const noexcept;

    /**
     * Makes the @ref Multimedia::Renderer Online with the passed device, e.g. when it's discovered again.
     * The details are refreshed from the device, the device must have the identity of the @ref Multimedia::Renderer.
     * An initialized @ref Multimedia::Renderer is initialized again for the new device.
     * The signals @ref Multimedia::Renderer::availabilityChanged and @ref Multimedia::Renderer::detailsChanged
     * are emitted when the Availability or the details changed.
     * @param mediaRenderer The @ref UPnPAV::MediaRenderer that shall be controlled by this @ref Multimedia::Renderer.
     */
    void goOnline(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer);

    /**
     * Makes the @ref Multimedia::Renderer Offline, e.g. when it leaves the network.
     * The @ref Multimedia::Renderer also goes Offline by itself when its device doesn't answer a call or its event
     * subscription anymore, a device that answers with an error stays Online.
     * The last known details are kept, the Playback State becomes No Media, the volume 0 and all requests are ignored
     * until the @ref Multimedia::Renderer is Online again.
     * The signal @ref Multimedia::Renderer::availabilityChanged is emitted when the Availability changed.
     */
    void goOffline() noexcept;

    /**
     * Initializes the @ref Mulitmedia::Renderer.
     * During the initialization the fetches all informations like, supported protocols for working properly.
     * Without calling the initialization the @ref Multimedia::Renderer does nothingl.
     * The signal @ref Mulitmedia::Renderer::initializationFinished is emitted on success.
     * The signal @ref Mulitmedia::Renderer::initializationFailed is emitted with a the error Information.
     */
    void initialize() noexcept;

    /**
     * Starts the playback of the passed @ref Multimedia::Item.
     * First checks that both @ref Multimedia::Renderer and @ref Multimedia::Item are compatible
     * Afterwards it calls the setAvTransportUri on the Renderer.
     * The signal @ref Multimedia::Renderer::playbackFailed with the error information is emitted on failure.
     */
    void playback(Item const& item) noexcept;

    /**
     * Stops or pause the current playback.
     * Pause is the preferred way to stop the playback but not every device support this feature.
     * So the pause is called when the device supports it in all other cases stop is called.
     * If the playback is not active nothing happens.
     * On success the signal @ref Multimedia::Renderer::stateChanged is emitted.
     * The new state then should be be stopped or paused.
     */
    void stop() noexcept;

    /**
     * Resumes the playback when the @ref Multimedia::Renderer has the state
     * @ref Mulitmedia::Renderer::State::Stopped
     * or
     * @ref Mulitmedia::Renderer::State::Paused.
     * If the device is in the stopped state the playback starts all over again and for the pause state
     * the track resumes at the pause state.
     * In all other cases the function does nothing.
     */
    void resume() noexcept;

    /**
     * Gives the state for the @ref Mulitmedia::Renderer.
     * @see The @ref Multimedia::Renderer::state for the meaing of the states.
     * @return The current state of the @ref Mulitmedia::MediaRenderer.
     */
    Renderer::State state() const noexcept;

    /**
     * @return The master volume of the active instance id.
     */
    quint32 volume() const noexcept;

    /**
     * Sets the volume for "Master" channel in UPnPAV renderer.
     * The result of volume is automatically propagated by the UPnPAV event system.
     * @param volume The volume for the "Master" channel.
     */
    void setVolume(quint32 volume) noexcept;

    /**
     * Gives the Current Track of the @ref Multimedia::Renderer.
     * The Current Track is taken from the events of the device right away and from its position info while the
     * position is tracked. It's empty while Offline.
     * @return The Current Track.
     */
    CurrentTrack const& currentTrack() const noexcept;

    /**
     * Switches the tracking of the position on or off, e.g. for the Active Renderer only.
     * While tracked and Playing, the @ref Multimedia::Renderer requests its position info every second, a request is
     * skipped while the previous one is pending. While tracked it also requests it right away after every Playback
     * State change, when it goes Online and when the tracking is switched on. Nothing is requested otherwise.
     * @param tracked True switches the tracking on, false switches it off.
     */
    void setPositionTracked(bool tracked) noexcept;

    /**
     * Gives whether the position is tracked.
     * @return True while the position is tracked.
     */
    bool isPositionTracked() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when @ref Multimedia::Renderer::initialize call finished successful.
     */
    void initializationFinished();

    /**
     * This signal is emitted when the @ref Multimedia::Renderer::initialize call has a failure.
     */
    void initializationFailed(QString const& errorMsg);

    /**
     * This signal is emitted when the @ref Multimedia::Renderer::playback call has a failure.
     */
    void playbackFailed(QString const& errorMsg);

    /**
     * This signal is emitted when the Renderer detects a state change of @ref UPnPAV::MediaRenderer
     * The actual state then can be requested with @ref Renderer::state.
     */
    void stateChanged();

    /**
     * This singal is emitted when the \"Master\" volume of the renderer for the current active intance id is changed.
     */
    void volumeChanged();

    /**
     * This signal is emitted when the @ref Multimedia::Renderer goes Online or Offline.
     */
    void availabilityChanged();

    /**
     * This signal is emitted when the name, manufacturer, model name or address changed.
     */
    void detailsChanged();

    /**
     * This signal is emitted when the Current Track changed.
     */
    void currentTrackChanged();

private Q_SLOTS:
    void onSetAvTransportUriFinished() noexcept;
    void onPlayCallFinished() noexcept;

private:
    bool isPlayableItem(Item const& item) const noexcept;
    void setState(UPnPAV::MediaRenderer::State state) noexcept;
    void updateVolume(quint32 volume) noexcept;
    void connectDevice() noexcept;
    std::unique_ptr<UPnPAV::PendingSoapCall> goOfflineWhenUnreachable(
        std::unique_ptr<UPnPAV::PendingSoapCall> call) noexcept;
    void onDeviceUnreachable() noexcept;
    void dropDevice() noexcept;
    void updateCurrentTrack(QString const& uri, QString const& metaData) noexcept;
    void setCurrentTrack(CurrentTrack const& track) noexcept;
    void requestPositionInfo() noexcept;
    void onPositionInfoFinished() noexcept;
    void updatePolling() noexcept;
    void onClockWokeUp() noexcept;

private:
    RememberedRenderer mRemembered;
    std::unique_ptr<UPnPAV::MediaRenderer> mRenderer;
    std::unique_ptr<UPnPAV::PendingSoapCall> mProtoInfoCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mSetAvTransportUriCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mPlayCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mStopCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mResumeCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mVolumeCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mSetVolumeCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mPositionInfoCall;
    bool mPositionInfoPending = false;
    std::unique_ptr<UPnPAV::Clock> mClock;
    bool mPositionTracked = false;
    bool mPolling = false;
    CurrentTrack mCurrentTrack;
    QString mCurrentTrackUri;
    QString mCurrentTrackMetaData;
    QStringList mSupportedTypes;
    QVector<UPnPAV::Protocol> mProtocols;
    Renderer::State mState = Renderer::State::NoMedia;
    quint32 mVolume = 0;
    bool mInitialized = false;
};

}; // namespace Multimedia
