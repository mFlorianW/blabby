# Source browsing is paged and re-fetched, not cached

A Source delivers the Items of a Container page by page as the user scrolls, because MediaServers silently cap
large Browse responses and a Container like "All Tracks" can hold thousands of Items. Going back up the tree
re-browses the parent Container up to the number of Items that were loaded and restores the scroll position, instead
of keeping earlier levels in memory, so the Library always shows the server's current content and the Source keeps
navigating by Path.

## Considered Options

- **Fetch all pages, then show**: simplest and always complete, but opening a huge Container blocks for a long time.
- **Cache each navigation level**: going back is instant, but the Source's navigation would change from Paths to
  cached Item lists and could show stale content.

## Consequences

- A page can fail after earlier pages succeeded; the Library keeps the loaded Items and offers an explicit retry.
- If the Container changed between pages, the latest total count wins and a rare duplicate or missing Item is accepted.
