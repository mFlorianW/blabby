// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import Blabby.Shell 1.0
import Blabby.Controls 1.0
import Blabby.Theme 1.0

Item{
    id: clock
    implicitHeight: clockUi.implicitHeight
    implicitWidth: clockUi.implicitWidth

    Text{
        id: clockUi
        text: Qt.formatTime(cppClock.currentTime , "hh:mm")
        font.family: Theme.fonts.bodyLarge.family
        font.pixelSize: Theme.fonts.bodyLarge.size
    }

    AbstractClock{
        id: cppClock

        onCurrentTimeChanged: () => {
            clockUi.text = Qt.formatTime(cppClock.currentTime , "hh:mm")
        }
    }
}
