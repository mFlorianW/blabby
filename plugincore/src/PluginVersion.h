// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef PLUGINVERSION_H
#define PLUGINVERSION_H

#include "blabbyplugincore_export.h"
#include <QtGlobal>

namespace PluginCore
{

/**
 * Struct which handles the version of the plugin
 */
struct BLABBYPLUGINCORE_EXPORT PluginVersion
{
    quint32 major = 0;
    quint32 minor = 0;
    quint32 patch = 0;
};

} // namespace PluginCore

#endif // PLUGINVERSION_H
