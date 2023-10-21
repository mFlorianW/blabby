<!--
SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>

SPDX-License-Identifier: GPL-2.0-or-later
-->
# Architecture {#Architecture}

## Building Blocks {#BuildingBlocks}

@startuml "Building Blocks"
!pragma layout elk
node "Blabby" {
    node "Core" {
        component UPnPAV as upnpav{
            [ServiceDiscovery]
            [MediaDevice]
        }

        component Multimedia as media {
            [Provider]
            [Multimedia]
            [Renderer]

            [Renderer] --> [MediaDevice]
            [Renderer] --> [ServiceDiscovery]

            [Multimedia] --> [Provider]
        }

    }

    node "Shell" {
        component Shell as shell
        shell --> [Multimedia]
        shell --> [Renderer]
        shell --> [Provider]
    }

    node "Plugins" {
        component Provider as provider{
            [MediaServerProvider]
            [FileSystemProvider]

            [MediaServerProvider] --> [Provider]
            [MediaServerProvider] --> [MediaDevice]
            [MediaServerProvider] --> [ServiceDiscovery]
            [FileSystemProvider] --> [Provider]
        }
    }
}
@enduml

## Multimedia {#Multimedia}
@startuml Provider Module
!pragma layout elk
node "Multimedia"{
    node "Provider"{
        interface Provider
        class ProviderLoader
        class MediaSource
        class MediaItem 
        class MediaItemModel
        class ProviderModel

        class ProviderLoader {
            void loadProvider(QStringList providerFolders);
            std::shared_pointer<ProviderModel> providerModel();
        }

        class Provider {
            QString name();
            Version verion();
            QVector<std::shared_pointer<MediaSource> sources();
            sourceAdded(std::shared_pointer<MediaSource> source);
            sourceRemoved(std::shared_pointer<MediaSource> source);
        }

        class MediaSource{
            QString name();
            MediaItemModel model();
            std::optional<Icon> icon();
        }

        class MediaItem{
            QString name();
            std::optional<Icon> icon();
            QUrl url();
            Type type();
        }

        ProviderLoader --> Provider : creates
        ProviderLoader "1" o-- "1" ProviderModel 
        Provider "1" o-- "1..*" MediaSource
        MediaSource "1" o-- "1" MediaItemModel
        MediaItemModel "1" o-- "*" MediaItem
    }

    node Multimedia {

    }

    node Renderer {

    }
}
@enduml

### Provider {#Provider}
The "Provider" module defines the interface for the media items.
It uses a plugin architecture in form of media sources.
The base for a plugin is the Provider interface.
A "Provider" can provide one to n media sourcen.
It is possible that at runtime a media source can appear or disappear.
Each provided media source must have a unique name, optional icon and version information.
Additionaly a media source must provide an item model to the actual media items.
A media item can have two different roles.
The first role is a container for other media items like album, folder etc.
The second role is the concrete media item that can be played by a media renderer.
