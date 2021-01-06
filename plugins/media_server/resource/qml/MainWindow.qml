import QtQuick 2.13
import QtQuick.Controls 2.15
import "views"

Item{
    id: mediaServerPlugin

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: mediaServerViewComp

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

    Component{
        id: mediaServerViewComp

        MediaServerView{
            id: mediaServerView
            model: g_MediaServerMinWindow.mediaServerModel

            onMediaServerActivated: {
                g_MediaServerMinWindow.setActiveMediaServer(index)
                stack.push(serverItemViewComp)
            }
        }
    }

    Component{
        id: serverItemViewComp

        ServerItemView{
            id: serverItemView

            Component.onCompleted: {
                g_ServerItemView.mediaServer = g_MediaServerMinWindow.activeMediaServer
            }
        }
    }

    Component.onCompleted:{
        g_MediaServerMinWindow.searchMediaServer()
    }
}
