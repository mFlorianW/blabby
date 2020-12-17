import QtQuick 2.13
import QtQuick.Controls 2.15
import de.blabby.mediaserverplugin 1.0
import "views"

MainController{
    id: mediaServerPlugin
    mediaServerModel: mediaServerModel
    mediaServerFactory: mediaServerFab
    serviceProviderFactory: serviceProviderFab

    StackView {
        id: stack
        initialItem: mediaServerViewComp
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
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

    Component{
        id: mediaServerViewComp

        MediaServerView{
            id: mediaServerView
            onMediaServerActivated: {
                console.info("ffff:" + index)
            }
        }
    }
}
