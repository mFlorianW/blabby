import QtQuick 2.0

Item {
    id: root

    property alias buttonText: buttonText.text
    property alias iconSource: pluginIcon.source
    property bool active: false

    Row{
        id: buttonRow
        anchors.fill: parent
        spacing: 10

        Rectangle{
            id: activeIndicator
            height: 29
            width: 8
            color: root.active ? "#dae1e7" : "#243242"
        }

        Image {
            id: pluginIcon
            width: 29
            height: 29
        }

        Text {
            id: buttonText
            font.pixelSize: 26
            color: "#dae1e7"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
