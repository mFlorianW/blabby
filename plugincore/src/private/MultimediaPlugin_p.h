// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MULTIMEDIAPLUGIN_P_H
#define MULTIMEDIAPLUGIN_P_H

#include <QUrl>

namespace PluginCore
{

class MultimediaPluginPrivate
{
public:
    QUrl mActiveView;
    bool mBackButtonNeeded{ false };
};

} // namespace PluginCore

#endif // MULTIMEDIAPLUGIN_P_H
