// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DummyPlugin.hpp"
namespace Multimedia
{

DummyPlugin::DummyPlugin()
    : Provider(QStringLiteral("DummyPlugin"), ProviderVersion{.major = 0, .minor = 1, .patch = 0})
{
}

DummyPlugin::~DummyPlugin() = default;

} // namespace Multimedia
