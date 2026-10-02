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

    /**
     * This property holds the position in the Current Track in milliseconds, 0 without an Active Renderer.
     */
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)

    /**
     * This property is true when the duration of the Current Track is known, it's unknown e.g. for a stream.
     */
    Q_PROPERTY(bool hasDuration READ hasDuration NOTIFY durationChanged)

    /**
     * This property holds the duration of the Current Track in milliseconds, 0 when unknown.
     */
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

    /**
     * This property is true when the Active Renderer can seek in the Current Track.
     */
    Q_PROPERTY(bool canSeek READ canSeek NOTIFY durationChanged)

    /**
     * This property holds the Volume of the Active Renderer, 0 without an Active Renderer.
     */
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)

    /**
     * This property holds the lowest Volume of the Active Renderer.
     */
    Q_PROPERTY(int volumeMinimum READ volumeMinimum NOTIFY activeRendererChanged)

    /**
     * This property holds the highest Volume of the Active Renderer.
     */
    Q_PROPERTY(int volumeMaximum READ volumeMaximum NOTIFY activeRendererChanged)

    /**
     * This property is true when the Volume of the Active Renderer can be controlled.
     */
    Q_PROPERTY(bool canControlVolume READ canControlVolume NOTIFY activeRendererChanged)

    /**
     * This property is true while the Active Renderer is muted, false without an Active Renderer.
     */
    Q_PROPERTY(bool muted READ isMuted NOTIFY muteChanged)

    /**
     * This property is true when the Mute of the Active Renderer can be controlled.
     */
    Q_PROPERTY(bool canControlMute READ canControlMute NOTIFY activeRendererChanged)

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
     * Gives the position in the Current Track in milliseconds, 0 without an Active Renderer.
     */
    qint64 position() const noexcept;

    /**
     * Gives true when the duration of the Current Track is known.
     */
    bool hasDuration() const noexcept;

    /**
     * Gives the duration of the Current Track in milliseconds, 0 when unknown.
     */
    qint64 duration() const noexcept;

    /**
     * Gives true when the Active Renderer can seek in the Current Track.
     */
    bool canSeek() const noexcept;

    /**
     * Seeks to the position in the Current Track of the Active Renderer.
     * @param position The position in milliseconds.
     */
    Q_INVOKABLE void seek(qint64 position) noexcept;

    /**
     * Gives the Volume of the Active Renderer, 0 without an Active Renderer.
     */
    int volume() const noexcept;

    /**
     * Gives the lowest Volume of the Active Renderer.
     */
    int volumeMinimum() const noexcept;

    /**
     * Gives the highest Volume of the Active Renderer.
     */
    int volumeMaximum() const noexcept;

    /**
     * Gives true when the Volume of the Active Renderer can be controlled.
     */
    bool canControlVolume() const noexcept;

    /**
     * Sets the Volume of the Active Renderer.
     * @param volume The Volume in the range of the Active Renderer.
     */
    Q_INVOKABLE void setVolume(int volume) noexcept;

    /**
     * Gives true while the Active Renderer is muted.
     */
    bool isMuted() const noexcept;

    /**
     * Gives true when the Mute of the Active Renderer can be controlled.
     */
    bool canControlMute() const noexcept;

    /**
     * Mutes or unmutes the Active Renderer, independent of its Volume.
     * @param muted True mutes, false unmutes.
     */
    Q_INVOKABLE void setMuted(bool muted) noexcept;

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
     * This signal is emitted when the position changes, also when the Active Renderer is changed or cleared.
     */
    void positionChanged();

    /**
     * This signal is emitted when the duration, and with it whether the Active Renderer can seek, changes, also when
     * the Active Renderer is changed or cleared.
     */
    void durationChanged();

    /**
     * This signal is emitted when the Volume changes, also when the Active Renderer is changed or cleared.
     */
    void volumeChanged();

    /**
     * This signal is emitted when the Mute changes, also when the Active Renderer is changed or cleared.
     */
    void muteChanged();

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
