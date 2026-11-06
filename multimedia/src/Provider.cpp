// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
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

QString const& Provider::name() const noexcept
{
    return mName;
}

ProviderVersion const& Provider::version() const noexcept
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

QString Provider::displayTitle() const noexcept
{
    return {};
}

QList<ProviderSetting> Provider::settings() const noexcept
{
    return {};
}

void Provider::setSettingValue(QString const& key, QVariant const& value) noexcept
{
    Q_UNUSED(key)
    Q_UNUSED(value)
}

void Provider::triggerAction(QString const& key) noexcept
{
    Q_UNUSED(key)
}

} // namespace Multimedia
