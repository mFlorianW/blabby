import QtQuick 2.12
import de.blabby.mediaserverplugin 1.0
import "../controls"

ListView {
    id:serverItemView

    model: g_ServerItemView.itemModel
    anchors.fill: parent
    anchors.topMargin: 55
    anchors.leftMargin: 35
    anchors.rightMargin: 55

    delegate: Item{
        width: serverItemView.width
        height: 82

        ServerItemButton {
            id: serverItemButton
            anchors.fill: parent
            anchors.topMargin: 12
            text: itemName
            iconSource: itemClass == ServerItemModel.ItemFolder ? p.folderIconSource : p.fileIconSource

            onClicked: {
                if(itemClass == ServerItemModel.ItemFolder){
                    g_ServerItemView.requestStorageFolder(itemId)
                }
            }
        }
    }

    QtObject {
        id: p

        readonly property string folderIconSource : "qrc:/mediaserver/icon/MediaServerFolder.png"
        readonly property string fileIconSource : "qrc:/mediaserver/icon/MediaServerItem.png"
    }
}
