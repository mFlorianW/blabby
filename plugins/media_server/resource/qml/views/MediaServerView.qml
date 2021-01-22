import QtQuick 2.0
import "../controls"

Item {
    id: mediaServerView

    GridView{
        id: mediaServerOverview

        anchors.fill: parent

        cellWidth: parent.width / 2
        cellHeight: 360
        model: g_MediaServerView.mediaServerModel

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
                    g_MediaServerPlugin.showServerItemView(g_MediaServerView.mediaServerModel.mediaServer(index))
                }
            }
        }
    }

    Component.onCompleted:{
        g_MediaServerView.searchMediaServer()
    }
}
