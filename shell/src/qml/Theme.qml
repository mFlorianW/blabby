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
        property color surfaceContainerHeighest: "#E1E3DF" 
    }

    property QtObject fonts: QtObject{
        property QtObject bodyLarge: QtObject{
            property int size: 16
            property string family: "Roboto"
        }
    }
}
