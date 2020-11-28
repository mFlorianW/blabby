import QtQuick 2.13
import de.blabby.mediaserverplugin 1.0
import "qrc:/mediaserver/qml/controls"

MainController{
    id: mediaServerPlugin
    mediaServerModel: mediaServerModel
    mediaServerFactory: mediaServerFab
    serviceProviderFactory: serviceProviderFab

    GridView{
        id: mediaServerOverview
        model: mediaServerModel

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
            }
        }
    }
    
    MediaServerModel{
        id: mediaServerModel
    }

    ServiceProviderFactory{
        id: serviceProviderFab
    }

    MediaServerFactory{
        id: mediaServerFab
    }

    Component.onCompleted:{
        mediaServerPlugin.searchMediaServer()
    }
}

