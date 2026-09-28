<!--
SPDX-FileCopyrightText: 2026 All contributors

SPDX-License-Identifier: GPL-2.0-or-later
-->
# Agent instructions

## Building and testing
Build and test through the CMake presets in `CMakePresets.json`, not with ad-hoc `cmake -S/-B` calls:

```sh
cmake --preset debug
cmake --build --preset debug
QT_QPA_PLATFORM=offscreen ctest --preset test          # or: ... ctest --preset test -R <test>
```

Use the `github-ci-debug` / `github-test` presets to reproduce CI.

Run everything that opens a UI (the QML tests, the shell app) headless with `QT_QPA_PLATFORM=offscreen`, so no
windows show up on the developer's desktop.

## Finishing a ticket
When implementing a GitHub issue (e.g. via /implement):

1. Before the first change, create a branch from an up-to-date `main`:
   `feature/issue<N>-<short-slug>` (e.g. `feature/issue214-ssdp-expiry`).
2. Commit on that branch; never commit ticket work directly to `main`.
3. When the review is done and the tests pass, push the branch and open a pull request against `main`:
   `gh pr create --base main --title "<ticket title>" --body "Closes #<N> …"`.
4. Do not merge the pull request; leave it for review and CI.

## Fixing linting errors
Findings from linters and static analysis (clang-tidy, clazy, clang-format, qmlformat, REUSE, …) are fixed
depending on where the offending code came from:

- **Introduced by a commit on the current branch** (not yet on `main`): commit the fix as a fixup of that commit,
  `git commit --fixup=<sha-of-the-commit-that-introduced-it>`.
- **Already on `main` or another branch**: commit the fix as a normal commit.

Before pushing, squash the fixup commits into their targets with
`GIT_SEQUENCE_EDITOR=: git rebase -i --autosquash origin/main` and, if the branch was already pushed, push with
`git push --force-with-lease`.
