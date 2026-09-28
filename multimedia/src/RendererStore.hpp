// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbymultimedia_export.h"
#include <QList>
#include <QString>

namespace Multimedia
{

/**
 * The details Blabby remembers of a @ref Multimedia::Renderer it has seen.
 */
struct BLABBYMULTIMEDIA_EXPORT RememberedRenderer
{
    /**
     * The identity of the @ref Multimedia::Renderer, the unique device name (UDN) of the device.
     */
    QString identity;

    /**
     * The last known name of the @ref Multimedia::Renderer.
     */
    QString name;

    /**
     * The last known manufacturer of the @ref Multimedia::Renderer, empty when the device doesn't name one.
     */
    QString manufacturer;

    /**
     * The last known model name of the @ref Multimedia::Renderer, empty when the device doesn't name one.
     */
    QString modelName;

    /**
     * The last known network address of the @ref Multimedia::Renderer, empty when it is unknown.
     */
    QString address;

    /**
     * Two remembered Renderers are equal when all their details are equal.
     */
    bool operator==(RememberedRenderer const& other) const = default;
};

/**
 * Keeps the @ref Multimedia::RememberedRenderer across restarts of Blabby.
 */
class BLABBYMULTIMEDIA_EXPORT RendererStore
{
public:
    /**
     * Default constructor
     */
    RendererStore() = default;

    /**
     * Default destructor
     */
    virtual ~RendererStore();

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(RendererStore)

    /**
     * Gives the remembered @ref Multimedia::Renderer.
     * @return The remembered Renderers or an empty list when none are remembered or they can't be read.
     */
    virtual QList<RememberedRenderer> load() const = 0;

    /**
     * Replaces the remembered @ref Multimedia::Renderer with the passed ones.
     * @param renderers The Renderers to remember.
     */
    virtual void save(QList<RememberedRenderer> const& renderers) = 0;
};

} // namespace Multimedia
