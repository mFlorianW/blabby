// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma Singleton
import QtQml
import QtQuick

/**
 * The Material 3 "Slate" dark scheme and type scale.
 * Controls read every colour and font from here and never use literal colour values, except "transparent".
 */
QtObject {
    /**
     * Material 3 colour roles of the Slate dark scheme.
     */
    readonly property QtObject colors: QtObject {
        readonly property color primary: "#B0C8EC"
        readonly property color colorOnPrimary: "#2A415F"
        readonly property color primaryContainer: "#3C5472"
        readonly property color colorOnPrimaryContainer: "#D4E4FF"

        readonly property color secondary: "#BBC7DB"
        readonly property color colorOnSecondary: "#354151"
        readonly property color secondaryContainer: "#313C4C"
        readonly property color colorOnSecondaryContainer: "#B4C0D4"

        readonly property color error: "#FA746F"
        readonly property color colorOnError: "#490006"

        readonly property color surface: "#0C0E12"
        readonly property color surfaceDim: "#0C0E12"
        readonly property color surfaceBright: "#282C34"
        readonly property color surfaceContainerLowest: "#000000"
        readonly property color surfaceContainerLow: "#111418"
        readonly property color surfaceContainer: "#161A1F"
        readonly property color surfaceContainerHigh: "#1C2026"
        readonly property color surfaceContainerHighest: "#21262D"
        readonly property color colorOnSurface: "#E2E5EF"
        readonly property color colorOnSurfaceVariant: "#A7ABB4"

        readonly property color outline: "#71767E"
        readonly property color outlineVariant: "#444850"
    }

    /**
     * Opacities of the Material 3 state layers drawn in the content colour over a control.
     */
    readonly property QtObject stateLayer: QtObject {
        readonly property real hoverOpacity: 0.08
        readonly property real pressedOpacity: 0.12
    }

    /**
     * Material 3 type scale.
     */
    readonly property QtObject fonts: QtObject {
        readonly property TypeStyle headlineMedium: TypeStyle {
            size: 28
            lineHeight: 36
        }
        readonly property TypeStyle titleLarge: TypeStyle {
            size: 22
            lineHeight: 28
        }
        readonly property TypeStyle titleMedium: TypeStyle {
            size: 16
            lineHeight: 24
            weight: Font.Medium
            letterSpacing: 0.15
        }
        readonly property TypeStyle bodyLarge: TypeStyle {
            size: 16
            lineHeight: 24
            letterSpacing: 0.5
        }
        readonly property TypeStyle bodyMedium: TypeStyle {
            size: 14
            lineHeight: 20
            letterSpacing: 0.25
        }
        readonly property TypeStyle labelLarge: TypeStyle {
            size: 14
            lineHeight: 20
            weight: Font.Medium
            letterSpacing: 0.1
        }
        readonly property TypeStyle labelMedium: TypeStyle {
            size: 12
            lineHeight: 16
            weight: Font.Medium
            letterSpacing: 0.5
        }
    }

    component TypeStyle: QtObject {
        property string family: "Roboto"
        property int size: 14
        property int lineHeight: 20
        property int weight: Font.Normal
        property real letterSpacing: 0
    }
}
