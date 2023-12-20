// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
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
    QString const &name() const noexcept;

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
     * The signal @ref Multimedia::Renderer::playbackStarted is emitted on success.
     * The signal @ref Multimedia::Renderer::playbackFailed with the error information is emitted on failure.
     */
    void playback(Item const &item) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when @ref Multimedia::Renderer::initialize call finished successful.
     */
    void initializationFinished();

    /**
     * This signal is emitted when the @ref Multimedia::Renderer::initialize call has a failure.
     */
    void initializationFailed(QString const &errorMsg);

    /**
     * This signal is emitted when the @ref Multimedia::Renderer::playback call finished successful.
     */
    void playbackStarted();

    /**
     * This signal is emitted when the @ref Multimedia::Renderer::playback call has a failure.
     */
    void playbackFailed(QString const &errorMsg);

private Q_SLOTS:
    void onSetAvTransportUriFinished() noexcept;
    void onPlayCallFinished() noexcept;

private:
    bool isPlayableItem(Item const &item) const noexcept;

private:
    std::unique_ptr<UPnPAV::MediaRenderer> mRenderer;
    std::unique_ptr<UPnPAV::PendingSoapCall> mProtoInfoCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mSetAvTransportUriCall;
    std::unique_ptr<UPnPAV::PendingSoapCall> mPlayCall;
    QStringList mSupportedTypes;
    QVector<UPnPAV::Protocol> mProtocols;
};

}; // namespace Multimedia
