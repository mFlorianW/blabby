// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "Item.hpp"

namespace Multimedia
{

Item::Item(ItemType type, QString mainText, QString secondaryText, QString iconUrl, QString path)
    : d{new ItemData{type, std::move(mainText), secondaryText, std::move(iconUrl), std::move(path)}}
{
}

ItemType Item::type() const noexcept
{
    return d->mType;
}

QString const &Item::mainText() const noexcept
{
    return d->mMainText;
}

QString const &Item::secondaryText() const noexcept
{

    return d->mSecondaryText;
}

QString const &Item::iconUrl() const noexcept
{
    return d->mIconUrl;
}

QString const &Item::path() const noexcept
{
    return d->mPath;
}

bool operator==(Item const &lhs, Item const &rhs) noexcept
{
    // clang-format off
    return (lhs.d == rhs.d) or ((lhs.d->mType == rhs.d->mType) and
                               (lhs.d->mMainText == rhs.d->mMainText) and
                               (lhs.d->mSecondaryText == rhs.d->mSecondaryText) and
                               (lhs.d->mIconUrl == rhs.d->mIconUrl) and
                               (lhs.d->mPath == rhs.d->mPath));
    // clang-format on
}

bool operator!=(Item const &lhs, Item const &rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace Multimedia
