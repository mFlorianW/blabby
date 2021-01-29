// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef FREEDLNACONTROL_EXPORT_H
#define FREEDLNACONTROL_EXPORT_H
#include <QtGlobal>

#if defined(BUILDING_UPNP)
#  define UPNP_EXPORT Q_DECL_EXPORT
#else
#  define UPNP_EXPORT Q_DECL_IMPORT
#endif

#endif // FREEDLNACONTROL_EXPORT_H
