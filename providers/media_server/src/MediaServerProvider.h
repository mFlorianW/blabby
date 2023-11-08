// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "Provider.h"
#include <memory>

namespace Provider
{

class MediaServerProvider : public Multimedia::Provider
{
    Q_OBJECT
    Q_INTERFACES(Multimedia::Provider)
    Q_PLUGIN_METADATA(IID "org.blabby.Provider")
public:
    MediaServerProvider();
    ~MediaServerProvider() override;

    Q_DISABLE_COPY_MOVE(MediaServerProvider)

private:
};

} // namespace Provider
