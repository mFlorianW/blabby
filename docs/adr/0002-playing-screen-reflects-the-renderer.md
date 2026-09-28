# The Playing screen reflects the Renderer, previous and next belong to Blabby's Queue

The Playing screen shows and controls the Current Track as the Active Renderer reports it, no matter which controller
started it, because a Renderer is shared on the network and the screen must not go stale when another app or the
device itself changes what plays. Previous and next are not sent to the Renderer's own AVTransport `Next`/`Previous`:
most Renderers only get a single URI and reject them, so stepping between tracks is left to Blabby's Queue.

## Considered Options

- **Show only what Blabby started, driven by its own Queue**: simpler state, but wrong as soon as someone else uses
  the Renderer, and the screen could not be built before playback from the Library and the Queue exist.
- **Wire previous and next to the Renderer's AVTransport actions**: works on the few Renderers that were given a
  playlist, fails on the rest, and would have to be rewired with different behaviour once the Queue exists.

## Consequences

- The Current Track and the playback position are read from the Renderer: position is polled every second while the
  Active Renderer is Playing, since UPnP does not event it.
- Previous, next, shuffle and repeat stay hidden until the Queue exists.
