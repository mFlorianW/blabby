// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererModel.hpp"
#include "Renderer.hpp"
#include <QObject>

namespace Shell
{

/**
 * The @ref Shell::ActiveRendererController follows the Active Renderer of a @ref Shell::MediaRendererModel and gives
 * the Playing screen what it shows of it. Without an Active Renderer it has no name, its Playback State is No Media
 * and it has no Current Track. The position of the Active Renderer is tracked, the one of the previous is not.
 */
class ActiveRendererController : public QObject
{
    Q_OBJECT

    /**
     * This property is true while there is an Active Renderer.
     */
    Q_PROPERTY(bool hasActiveRenderer READ hasActiveRenderer NOTIFY activeRendererChanged)

    /**
     * This property holds the name of the Active Renderer, empty without an Active Renderer.
     */
    Q_PROPERTY(QString rendererName READ rendererName NOTIFY activeRendererChanged)

    /**
     * This property holds the Playback State of the Active Renderer, No Media without an Active Renderer.
     */
    Q_PROPERTY(Multimedia::Renderer::State playbackState READ playbackState NOTIFY playbackStateChanged)

    /**
     * This property holds the title of the Current Track, empty without an Active Renderer.
     */
    Q_PROPERTY(QString trackTitle READ trackTitle NOTIFY currentTrackChanged)

    /**
     * This property holds the artist of the Current Track, empty when unknown.
     */
    Q_PROPERTY(QString trackArtist READ trackArtist NOTIFY currentTrackChanged)

    /**
     * This property holds the URL of the artwork of the Current Track, empty when unknown.
     */
    Q_PROPERTY(QString artworkUrl READ artworkUrl NOTIFY currentTrackChanged)

    /**
     * This property holds the album of the Current Track, empty when unknown.
     */
    Q_PROPERTY(QString trackAlbum READ trackAlbum NOTIFY currentTrackChanged)

    /**
     * This property holds the year of the Current Track, empty when unknown.
     */
    Q_PROPERTY(QString trackYear READ trackYear NOTIFY currentTrackChanged)

    /**
     * This property holds the format of the Current Track, e.g. "FLAC · 24-bit / 96 kHz", empty when unknown.
     */
    Q_PROPERTY(QString trackFormat READ trackFormat NOTIFY currentTrackChanged)

    /**
     * This property is true when the Active Renderer can pause, otherwise it is stopped instead.
     */
    Q_PROPERTY(bool canPause READ canPause NOTIFY activeRendererChanged)

    /**
     * This property is true while the Active Renderer is transitioning, e.g. loading or buffering.
     */
    Q_PROPERTY(bool transitioning READ isTransitioning NOTIFY transitioningChanged)

public:
    /**
     * Creates an instance of the @ref Shell::ActiveRendererController that follows the Active Renderer of the model.
     * @param model The model that holds the Active Renderer, it must outlive the controller.
     */
    explicit ActiveRendererController(MediaRendererModel const& model);

    /**
     * Default destructor
     */
    ~ActiveRendererController() override;

    /*
     * Disabled copy and move semantic.
     */
    Q_DISABLE_COPY_MOVE(ActiveRendererController)

    /**
     * Gives true while there is an Active Renderer.
     */
    bool hasActiveRenderer() const noexcept;

    /**
     * Gives the name of the Active Renderer, empty without an Active Renderer.
     */
    QString rendererName() const noexcept;

    /**
     * Gives the Playback State of the Active Renderer, No Media without an Active Renderer.
     */
    Multimedia::Renderer::State playbackState() const noexcept;

    /**
     * Gives the title of the Current Track, empty without an Active Renderer.
     */
    QString trackTitle() const noexcept;

    /**
     * Gives the artist of the Current Track, empty when unknown.
     */
    QString trackArtist() const noexcept;

    /**
     * Gives the URL of the artwork of the Current Track, empty when unknown.
     */
    QString artworkUrl() const noexcept;

    /**
     * Gives the album of the Current Track, empty when unknown.
     */
    QString trackAlbum() const noexcept;

    /**
     * Gives the year of the Current Track, empty when unknown.
     */
    QString trackYear() const noexcept;

    /**
     * Gives the format of the Current Track, empty when unknown.
     */
    QString trackFormat() const noexcept;

    /**
     * Gives true when the Active Renderer can pause.
     */
    bool canPause() const noexcept;

    /**
     * Gives true while the Active Renderer is transitioning.
     */
    bool isTransitioning() const noexcept;

    /**
     * Pauses a Playing Active Renderer, or stops it when it can't pause, and resumes a Paused or plays a Stopped one.
     * The request is ignored while a previous one is pending or the Active Renderer is transitioning.
     */
    Q_INVOKABLE void togglePlayback() noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the Active Renderer is changed or cleared.
     */
    void activeRendererChanged();

    /**
     * This signal is emitted when the Playback State changes, also when the Active Renderer is changed or cleared.
     */
    void playbackStateChanged();

    /**
     * This signal is emitted when the Current Track changes, also when the Active Renderer is changed or cleared.
     */
    void currentTrackChanged();

    /**
     * This signal is emitted when the Active Renderer starts or stops transitioning, also when the Active Renderer is
     * changed or cleared.
     */
    void transitioningChanged();

    /**
     * This signal is emitted when a control call to the Active Renderer failed.
     * @param rendererName The name of the Active Renderer.
     * @param action The action of the failed call.
     */
    void controlFailed(QString const& rendererName, Multimedia::Renderer::Action action);

    /**
     * This signal is emitted when the Active Renderer went Offline and is no longer the Active Renderer.
     * @param rendererName The name of the Renderer that went Offline.
     */
    void activeRendererWentOffline(QString const& rendererName);

private:
    void onActiveRendererChanged();
    Multimedia::CurrentTrack currentTrack() const noexcept;

    MediaRendererModel const& mModel;
    std::shared_ptr<Multimedia::Renderer> mRenderer = nullptr;
};

} // namespace Shell
