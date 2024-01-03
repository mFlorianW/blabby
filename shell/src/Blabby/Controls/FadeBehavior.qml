// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.15

Behavior {
    id: root
    property Item fadeTarget: targetProperty.object
    SequentialAnimation {
        NumberAnimation {
            target: root.fadeTarget
            property: "opacity"
            to: 0
            easing.type: Easing.InQuad
        }
        PropertyAction { }
        NumberAnimation {
            target: root.fadeTarget
            property: "opacity"
            to: 1
            easing.type: Easing.OutQuad
        }
    }
}
