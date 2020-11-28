import QtQuick 2.13

Rectangle{
    id: root
    color: "#87949f"
    implicitWidth: 284
    implicitHeight: 284

    property alias name: serverName.text
    property alias iconSource : mediaServerIcon.source

    Item{
        id: iconContainer
        width: parent.width
        height: parent.height - buttonRect.height

        Image{
            id: mediaServerIcon
            anchors.centerIn: parent
            height: 120
            width: 120
        }
    }

    Rectangle{
        id: buttonRect
        anchors.bottom: parent.bottom
        width: parent.width
        height: 60
        color: "#00909e"

        Text{
            id: serverName
            anchors.left: parent.left
            anchors.leftMargin: 32
            anchors.right: parent.right
            anchors.rightMargin: 32
            anchors.verticalCenter: parent.verticalCenter
            color: "#dae1e7"
            font.pixelSize: 24
            elide: Text.ElideRight
        }
    }
}