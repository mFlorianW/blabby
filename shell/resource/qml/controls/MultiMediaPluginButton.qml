import QtQuick 2.0

Item {
    id: root

    property alias buttonText: buttonText.text

    Text {
        id: buttonText
        anchors.centerIn: parent
        font.pixelSize: 28
        color: "#dae1e7"
    }
}
