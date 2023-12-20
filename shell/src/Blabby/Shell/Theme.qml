// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
pragma Singleton
import QtQml
import QtQuick

QtObject{
    property QtObject colors: QtObject{
        property color surface: "#F8FAF6"
        property color surfaceContainerHigh: "#E7E9E5"
        property color surfaceContainerHeighest: "#E1E3DF"
        property color outline: "#707973"
    }

    property QtObject stateColors: QtObject{
       property QtObject onSurfaceVariant: QtObject{
            property color opacity08: "#404943"
            property color opacity12: opacity08
       }
    }

    property QtObject fonts: QtObject{
        property QtObject bodyLarge: QtObject{
            property int size: 16
            property string family: "Roboto"
            property color color: "#191C1A"
            property bool bold: false
        }
        property QtObject titleMedium: QtObject{
            property int size: 16
            property string family: "Roboto"
            property color color: "#404943"
            property bool bold: true
        }
        property QtObject titleLarge: QtObject{
            property int size: 22
            property string family: "Roboto"
            property color color: "#404943"
            property bool bold: true
        }
    }
}
