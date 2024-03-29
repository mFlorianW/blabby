// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Item.hpp"
#include "MediaRenderer.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>

namespace Multimedia
{

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
     * Creates an instance of the Renderer
     * @param mediaRenderer The @ref UPnPAV::MediaRenderer that shall be controlled by this @ref Multimedia::Renderer.
     */
    Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer);

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

private Q_SLOTS:
    void onSetAvTransportUriFinished() noexcept;
    void onPlayCallFinished() noexcept;

private:
    bool isPlayableItem(Item const& item) const noexcept;
    void setState(UPnPAV::MediaRenderer::State state) noexcept;

private:
    std::unique_ptr<UPnPAV::MediaRenderer> mRenderer;
    std::unique_ptr<UPnPAV::PendingSoapCall> mProtoInfoCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mSetAvTransportUriCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mPlayCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mStopCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mResumeCall;
    QStringList mSupportedTypes;
    QVector<UPnPAV::Protocol> mProtocols;
    Renderer::State mState = Renderer::State::NoMedia;
};

}; // namespace Multimedia
