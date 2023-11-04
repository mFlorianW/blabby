// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "DummyPlugin.h"
namespace Multimedia
{

DummyPlugin::DummyPlugin()
    : Provider(QStringLiteral("DummyPlugin"), ProviderVersion{.major = 0, .minor = 1, .patch = 0})
{
}

DummyPlugin::~DummyPlugin() = default;

} // namespace Multimedia
