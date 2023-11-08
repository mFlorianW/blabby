// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerProvider.h"

namespace Provider
{

MediaServerProvider::MediaServerProvider()
    : Provider{QStringLiteral("MediaServerProvider"), Multimedia::ProviderVersion{.major = 0, .minor = 1, .patch = 1}}
{
}

MediaServerProvider::~MediaServerProvider() = default;

} // namespace Provider
