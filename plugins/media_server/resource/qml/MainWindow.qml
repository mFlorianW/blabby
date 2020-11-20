import QtQuick 2.13

Text {
    id: welcomeText
    color: "#7f8c8d"
    font.pixelSize: 72
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: parent.verticalCenter
    text: qsTr("Welcome to \nMediaServer")
}

