// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

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
    ~Renderer() override = default;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(Renderer)

private:
    std::unique_ptr<UPnPAV::MediaRenderer> mRenderer;
};

}; // namespace Multimedia
