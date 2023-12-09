// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "MediaRenderer.hpp"

namespace UPnPAV::Doubles
{

class MediaRenderer : public UPnPAV::MediaRenderer
{
public:
    using UPnPAV::MediaRenderer::MediaRenderer;
    ~MediaRenderer() override;
    Q_DISABLE_COPY_MOVE(MediaRenderer)
};

} // namespace UPnPAV::Doubles
