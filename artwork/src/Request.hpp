// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbyartwork_export.h"
#include <QImage>
#include <QObject>

namespace Artwork
{

/**
 * The @ref Artwork::Request is the handle of a request for the Artwork of an album.
 * It finishes once and asynchronously, with an image or with "no Artwork". It can be cancelled until then.
 */
class BLABBYARTWORK_EXPORT Request : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Request)
public:
    Request();
    ~Request() override;

    /**
     * Gives true when the request is finished.
     */
    bool isFinished() const noexcept;

    /**
     * Gives true when the request is cancelled.
     */
    bool isCancelled() const noexcept;

    /**
     * Gives the Artwork, a null image when the request finished with "no Artwork" or isn't finished.
     */
    QImage const& image() const noexcept;

    /**
     * Cancels the request, it won't finish anymore. Does nothing when the request is already finished.
     */
    void cancel() noexcept;

    /**
     * Finishes the request and emits @ref finished. Called by the one serving the request, it does nothing when the
     * request is finished or cancelled.
     * @param image The Artwork, a null image for "no Artwork".
     */
    void finish(QImage image);

Q_SIGNALS:
    /**
     * This signal is emitted when the request is finished.
     */
    void finished();

    /**
     * This signal is emitted when the request is cancelled.
     */
    void cancelled();

private:
    enum class State
    {
        Running,
        Finished,
        Cancelled,
    };

    State mState = State::Running;
    QImage mImage;
};

} // namespace Artwork
