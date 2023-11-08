// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaServerSource.h"

namespace Provider
{

MediaServerSource::MediaServerSource(std::unique_ptr<UPnPAV::MediaServer> mediaServer)
    : Multimedia::MediaSource{mediaServer->name(), mediaServer->iconUrl().toString()}
    , mServer{std::move(mediaServer)}
{
}

MediaServerSource::~MediaServerSource() = default;

} // namespace Provider
