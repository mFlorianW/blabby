# Blabby

Blabby is a controller for UPnP speakers on the local network. It lets a user browse media offered by Sources and play it on a Renderer, typically from a Raspberry Pi with a touch display.

## Language

### Browsing media

**Provider**:
A plugin that offers zero or more Sources and reports them as they appear or disappear at runtime.
_Avoid_: Plugin (when meaning the domain concept), backend

**Source**:
A named, browsable collection of Items offered by a Provider, such as a MediaServer on the network or a mounted USB stick.
_Avoid_: Library, repository

**Active Source**:
The one Source whose Items the user is currently browsing. At most one Source is active at a time; there is none until the user picks one, and none again when the Active Source disappears.
_Avoid_: Current source, selected source, open source

**Item**:
A single entry inside a Source, either a Container or a Playable.
_Avoid_: Entry, element, track (for Containers)

**Container**:
An Item that groups other Items, such as a folder or an album; opening it navigates deeper into the Source.
_Avoid_: Folder, directory (as the general term)

**Playable**:
An Item that can be played on a Renderer, such as a song.
_Avoid_: File, track (as the general term)

**Path**:
The location of an Item within its Source, whose format is defined by the Source.
_Avoid_: URL, address

### Playing media

**Renderer**:
A device Blabby knows about that plays Playables and exposes their playback state and volume. A Renderer is remembered once seen and is recognised by its device identity when it reappears, even under a new name or address.
_Avoid_: Speaker, player, sink, device

**Availability**:
Whether a Renderer is currently reachable on the network: Online or Offline. Only an Online Renderer can play.
_Avoid_: Status, connection state

**Forget**:
The user removing an Offline Renderer from the Renderers Blabby remembers. A forgotten Renderer that comes back Online is remembered again as if newly seen.
_Avoid_: Delete, remove

**Active Renderer**:
The one Renderer that Blabby currently sends playback to and controls. At most one Renderer is active at a time; there is none until the user picks one, and none again when the Active Renderer goes Offline.
_Avoid_: Selected renderer, current renderer, output, target

**Playback State**:
The current condition of a Renderer: No Media, Stopped, Playing or Paused.
_Avoid_: Status, transport state

**Volume**:
The loudness of a Renderer's master channel.
_Avoid_: Gain, level

### UPnP AV

**MediaServer**:
A UPnP AV device on the network that shares media; Blabby presents each one as a Source.

**MediaRenderer**:
A UPnP AV device on the network that plays media; Blabby presents each one as a Renderer.
