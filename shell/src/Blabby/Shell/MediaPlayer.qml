// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Singleton 1.0

Item {
    id: mediaPlayer

    IconButton {
        id: playerStateButton
        anchors.centerIn: parent
        style: AbstractIconButton.Style.Tonal
        source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg"
    }
}
