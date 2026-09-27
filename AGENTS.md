<!--
SPDX-FileCopyrightText: 2026 All contributors

SPDX-License-Identifier: GPL-2.0-or-later
-->
# Agent instructions

## Finishing a ticket
When implementing a GitHub issue (e.g. via /implement):

1. Before the first change, create a branch from an up-to-date `main`:
   `feature/issue<N>-<short-slug>` (e.g. `feature/issue214-ssdp-expiry`).
2. Commit on that branch; never commit ticket work directly to `main`.
3. When the review is done and the tests pass, push the branch and open a pull request against `main`:
   `gh pr create --base main --title "<ticket title>" --body "Closes #<N> …"`.
4. Do not merge the pull request; leave it for review and CI.
