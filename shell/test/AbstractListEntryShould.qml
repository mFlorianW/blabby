import QtQuick
import QtTest
import Blabby.Controls

Item{
    id: root
    height: 400
    width: root.height

    Rectangle{
        id: trailingElem
        height: 80
        width: 80
        color: "black"
    }

    AbstractListEntry{
        id: abstractListEntry
        anchors.fill: parent
        trailingElement: trailingElem
    }

    TestCase {
        id: abstractListEntryTest
        when: windowShown

        /**
         * Tests that the trailing item is correctly passed to qml.
         */
        function test_trailing_item_is_correctly_passed_to_qml(){
            compare(abstractListEntry.trailingElement, trailingElem)
        }
    }
}
