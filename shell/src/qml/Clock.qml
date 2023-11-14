// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import BlabbyShell 0.0

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
