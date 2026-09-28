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
