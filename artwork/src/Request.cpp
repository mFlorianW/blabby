// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Request.hpp"

namespace Artwork
{

Request::Request() = default;
Request::~Request() = default;

bool Request::isFinished() const noexcept
{
    return mState == State::Finished;
}

bool Request::isCancelled() const noexcept
{
    return mState == State::Cancelled;
}

QImage const& Request::image() const noexcept
{
    return mImage;
}

void Request::cancel() noexcept
{
    if (mState == State::Running) {
        mState = State::Cancelled;
        Q_EMIT cancelled();
    }
}

void Request::finish(QImage image)
{
    if (mState == State::Running) {
        mState = State::Finished;
        mImage = std::move(image);
        Q_EMIT finished();
    }
}

} // namespace Artwork
