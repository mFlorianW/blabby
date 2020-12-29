import QtQuick 2.12
import de.blabby.mediaserverplugin 1.0
import "../controls"

ServerItemController{
    id: serverItemController
    itemModel: serverItemModel

    ListView {
        id:serverItemView

        model: serverItemModel
        anchors.fill: parent
        anchors.topMargin: 55
        anchors.leftMargin: 35
        anchors.rightMargin: 55


        delegate: Item{
            anchors.left: parent.left
            anchors.right: parent.right
            height: 82

            ServerItemButton {
                id: serverItemButton
                anchors.fill: parent
                anchors.topMargin: 12
                text: itemName
                iconSource:  itemClass == ServerItemModel.ItemFolder ? p.folderIconSource : p.fileIconSource

                onClicked: {
                    serverItemController.requestStorageFolder(itemId)
                }
            }
        }
    }

    ServerItemModel{
        id: serverItemModel
    }

    QtObject {
        id: p

        readonly property string folderIconSource : "qrc:/mediaserver/icon/MediaServerFolder.png"
        readonly property string fileIconSource : "qrc:/mediaserver/icon/MediaServerItem.png"
    }
}
