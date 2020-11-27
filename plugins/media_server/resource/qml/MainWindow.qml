import QtQuick 2.13
import de.blabby.mediaserverplugin 1.0

MainController{
    id: root
    mediaServerModel: mediaServerModel
    mediaServerFactory: mediaServerFab
    serviceProviderFactory: serviceProviderFab

    GridView{
        id: mediaServerOverview
        model: mediaServerModel
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
}

