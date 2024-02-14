// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Singleton 1.0
import Blabby.Objects 1.0 as Obj

Item {
    id: mediaPlayer

    IconButton {
        id: playerStateButton
        anchors.centerIn: parent
        style: AbstractIconButton.Style.Tonal
        source: Singleton.mediaPlayer.playbackState === Obj.MediaPlayer.Playing ? "qrc:/qt/qml/Blabby/Shell/icons/24x24/pause.svg" : "qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg"
        onClicked: Singleton.mediaPlayer.playbackState === Obj.MediaPlayer.Playing ? Singleton.mediaPlayer.stop() : Singleton.mediaPlayer.resume()
    }
}
