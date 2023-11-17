// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick

Rectangle{
    id: menu

    implicitWidth: 300
    color: Theme.colors.surface

    Rectangle{
        id: header
        height: 125
        width: menu.width
        color: Theme.colors.surfaceContainerHeighest
    }
}
