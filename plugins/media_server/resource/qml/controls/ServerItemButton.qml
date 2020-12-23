import QtQuick 2.12

Rectangle {
    id:root
    color: "#87949f"

    property alias text : itemText.text

    Text {
        id: itemText
        anchors.left: parent.left
        anchors.leftMargin: 90
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 30
        color: "#dae1e7"
    }
}
