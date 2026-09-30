// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Theme

Item {
    id: root
    width: 200
    height: 200

    IconBadge {
        id: badge
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
    }

    TestCase {
        id: badgeTest
        name: "IconBadgeShould"
        when: windowShown

        function init() {
            badge.emphasis = false;
            badge.width = badge.implicitWidth;
            badge.height = badge.implicitHeight;
        }

        /**
         * Tests that the icon takes half of the badge, 24 px in the default badge and 20 px in a 40 px badge.
         */
        function test_scale_the_icon_with_the_badge() {
            const icon = badgeTest.findChild(badge, "icon");
            badgeTest.compare(icon.width, 24);
            badge.width = 40;
            badge.height = 40;
            badgeTest.compare(icon.width, 20);
            badgeTest.compare(icon.height, 20);
            badgeTest.compare(badge.radius, 20);
        }

        /**
         * Tests that the badge shows the given icon.
         */
        function test_show_the_icon() {
            const icon = badgeTest.findChild(badge, "icon");
            badgeTest.verify(icon);
            badgeTest.compare(icon.source, badge.iconSource);
        }

        /**
         * Tests that an emphasised badge is drawn in the primary colour.
         */
        function test_draw_the_emphasised_badge_in_the_primary_colour() {
            const icon = badgeTest.findChild(badge, "icon");
            badgeTest.verify(Qt.colorEqual(badge.color, Theme.colors.surfaceContainerHighest));
            badgeTest.verify(Qt.colorEqual(icon.color, Theme.colors.colorOnSurfaceVariant));
            badge.emphasis = true;
            badgeTest.verify(Qt.colorEqual(badge.color, Theme.colors.primary));
            badgeTest.verify(Qt.colorEqual(icon.color, Theme.colors.colorOnPrimary));
        }
    }
}
