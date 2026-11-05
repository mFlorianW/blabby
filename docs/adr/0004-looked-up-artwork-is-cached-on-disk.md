# Looked-up Artwork is cached on disk

When a Source supplies no Artwork for an album, Blabby looks it up on the internet (MusicBrainz and the Cover Art
Archive). The images it finds are kept on disk across restarts, unlike Source browsing (ADR 0001), because the
lookup is slow, limited to one request per second, depends on a third party, and an album's Artwork practically never
changes once it exists.

## Considered Options

- **No cache**: always current, but every restart and every scroll repeats rate-limited lookups, and the placeholder
  shows for seconds on a Library full of albums.
- **In-memory cache only**: no SD card writes, but every restart starts the slow lookups over.

## Consequences

- Images are scaled down to the largest size Blabby shows before being written, to keep SD card writes and disk
  use small.
- "No Artwork found" is not written to disk; it is remembered only while Blabby runs, so an album is not looked up
  on every scroll, but every start tries again and an album that gains Artwork online shows it right away.
