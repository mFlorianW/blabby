import QtQuick 2.0
import "../controls"

Item {
    id: mediaServerView

    signal mediaServerActivated(int index)
    property alias model: mediaServerOverview.model

    GridView{
        id: mediaServerOverview

        anchors.fill: parent

        cellWidth: parent.width / 2
        cellHeight: 360

        delegate: Item {
            id: cellContainer
            width: mediaServerOverview.cellWidth
            height: mediaServerOverview.cellHeight
            MediaServerButton{
                name: mediaServerName
                iconSource: mediaServerIconUrl
                width: 284
                height: 284
                anchors.centerIn: parent

                onClicked: {
                    mediaServerActivated(index)
                }
            }
        }
    }
}
