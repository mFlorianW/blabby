<!--
SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>

SPDX-License-Identifier: GPL-2.0-or-later
-->
# Architecture {#Architecture}

## Building Blocks {#BuildingBlocks}

@startuml "Building Blocks"
node "Blabby" {
    node "Core" {
        component UPnPAV as upnpav{
            [ServiceDiscovery]
            [MediaDevice]
        }

        component MultiMedia as media {
            [ProviderPlugin]
            [MultiMedia]
            [Renderer]

            [Renderer] --> [MediaDevice]
            [Renderer] --> [ServiceDiscovery]

            [MultiMedia] --> [ProviderPlugin]
        }

        component Shell as shell
        shell --> [MultiMedia]
        shell --> [Renderer]
    }
}
@enduml
