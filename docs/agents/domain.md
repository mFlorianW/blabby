# Domain Docs

How the engineering skills should consume this repo's domain documentation when exploring the codebase.

## Before exploring, read these

- **`CONTEXT.md`** at the repo root: the glossary (this repo's name for `GLOSSARY.md`).
- **The ADRs in the [project wiki](https://github.com/mFlorianW/blabby/wiki/Architecture-Decision-Records)**: read the
  ones that touch the area you're about to work in. See "ADRs live in the wiki" below for how to get them.

If any of these files don't exist, **proceed silently**. Don't flag their absence; don't suggest creating them upfront. The `/domain-modeling` skill (reached via `/grill-with-docs` and `/improve-codebase-architecture`) creates them lazily when terms or decisions actually get resolved.

## File structure

Single-context repo:

```
/
├── CONTEXT.md
└── <component>/src/
```

## ADRs live in the wiki

ADRs are kept and maintained only in the GitHub wiki of mFlorianW/blabby, never in the repository. Don't create
`docs/adr/`, even if a skill says so; this section replaces any skill's instructions on where and how ADRs are stored.

The wiki is a separate git repository. Clone it into your scratchpad to read or change ADRs:

```sh
git clone git@github.com:mFlorianW/blabby.wiki.git <scratchpad>/wiki
```

- `Architecture-Decision-Records.md` is the index: a table of all ADRs, a short summary of each and how to write a
  new one.
- Each ADR is a page `ADR-NNNN-Title-Case-Slug.md` titled `# ADR NNNN: <the decision>`.

To record a new ADR, follow "Writing a new ADR" on the index page: add the page with the next free number, add it to
the index table and summary and to `_Sidebar.md`. Then commit and push to the wiki's `master` branch. Wiki pushes are
published immediately and don't go through a pull request, so show the user the new page and ask before pushing.

## Use the vocabulary of `CONTEXT.md`

When your output names a domain concept (in an issue title, a refactor proposal, a hypothesis, a test name), use the term as defined in `CONTEXT.md`. Don't drift to synonyms the glossary explicitly avoids.

If the concept you need isn't in `CONTEXT.md` yet, that's a signal: either you're inventing language the project doesn't use (reconsider) or there's a real gap (note it for `/domain-modeling`).

## Flag ADR conflicts

If your output contradicts an existing ADR, surface it explicitly rather than silently overriding:

> _Contradicts ADR-0007 (event-sourced orders), but worth reopening because…_
