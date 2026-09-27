// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 800
    height: 200

    ScreenHeader {
        id: header
        width: root.width
        title: "Renderers"
        subtitle: "Choose where your music plays"

        Rectangle {
            id: action
            implicitWidth: 40
            implicitHeight: 40
        }
    }

    TestCase {
        id: headerTest
        name: "ScreenHeaderShould"
        when: windowShown

        /**
         * Tests that the title and subtitle are displayed.
         */
        function test_show_title_and_subtitle() {
            headerTest.compare(headerTest.findChild(header, "title").text, "Renderers");
            headerTest.compare(headerTest.findChild(header, "subtitle").text, "Choose where your music plays");
        }

        /**
         * Tests that the trailing action is placed at the right edge of the header.
         */
        function test_place_the_trailing_action_at_the_right_edge() {
            const actionRight = action.mapToItem(header, action.width, 0).x;
            headerTest.verify(actionRight <= header.width);
            headerTest.verify(actionRight >= header.width - 48);
            headerTest.verify(action.mapToItem(header, 0, 0).x > headerTest.findChild(header, "title").width);
        }
    }
}
