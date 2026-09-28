# Blabby advances the Queue itself, one Playable at a time

Blabby gives the Active Renderer only the Current Entry of the Queue and sends the next one itself once the Renderer
has finished it, instead of pre-loading the next Playable with AVTransport `SetNextAVTransportURI`. The action is
optional in UPnP and missing on many inexpensive Renderers, so a single mechanism that works on every Renderer that
can play at all is preferred over gapless playback on some of them. A Renderer counts as finished when it reports
Stopped while its Current Track is still the Current Entry and the last known position was close to the end.

## Considered Options

- **Pre-load the next Playable with `SetNextAVTransportURI`**: gapless and needs no end detection, but only on
  Renderers that support it, so the self-advancing path is needed anyway.
- **Pre-load where supported, advance otherwise**: best result per Renderer, but two code paths to build and test
  from the start.

## Consequences

- A short gap between Playables is accepted.
- The Queue only advances while Blabby runs and follows the Active Renderer.
- Another controller stopping the Renderer mid-Playable does not advance the Queue: a stop on the device means stop.
- A Playable without a duration, such as a stream, never finishes on its own; if it drops, the Queue stays on it.
- Pre-loading can be added later as an improvement for Renderers that support it.
