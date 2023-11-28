// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "TestMediaSource.hpp"

namespace Multimedia::TestHelper
{

TestMediaSource::TestMediaSource(QString name, QString iconUrl)
    : Multimedia::MediaSource{std::move(name), std::move(iconUrl)}
{
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem1")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem2")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Container, QStringLiteral("Container1")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem3")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem4")});
}

TestMediaSource::~TestMediaSource() = default;

} // namespace Multimedia::TestHelper
