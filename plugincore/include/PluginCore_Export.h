// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef PLUGINCORE_EXPORT_H
#define PLUGINCORE_EXPORT_H

#include <QtGlobal>

#if defined(PLUGINCORE)
#  define PLUGINCORE_EXPORT Q_DECL_EXPORT
#else
#  define PLUGINCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINCORE_EXPORT_H
