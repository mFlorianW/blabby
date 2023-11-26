
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "Provider.hpp"

namespace Multimedia
{

QString ProviderVersion::toString() const noexcept
{
    return QString("%1.%2.%3").arg(QString::number(major), QString::number(minor), QString::number(patch));
}

Provider::Provider(QString name, ProviderVersion version)
    : mName{std::move(name)}
    , mVersion{std::move(version)}
{
}

Provider::~Provider() = default;

const QString &Provider::name() const noexcept
{
    return mName;
}

ProviderVersion const &Provider::version() const noexcept
{
    return mVersion;
}

bool Provider::init() noexcept
{
    return true;
}

MediaSources Provider::sources() const noexcept
{
    return {};
}

} // namespace Multimedia
