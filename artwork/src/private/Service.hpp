// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Request.hpp"
#include <QObject>
#include <memory>

namespace Artwork
{

/**
 * The @ref Artwork::Service is an online service that knows the Artwork of albums.
 */
class Service : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Service)
public:
    ~Service() override;

    /**
     * Looks the Artwork of an album up.
     * @param artist The artist of the album, empty when unknown.
     * @param album The title of the album.
     * @return The request, it finishes asynchronously with the Artwork or with "no Artwork". Cancelling it stops the
     *         lookup.
     */
    virtual std::shared_ptr<Request> lookUp(QString const& artist, QString const& album) = 0;

protected:
    Service() = default;
};

} // namespace Artwork
