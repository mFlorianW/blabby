<!--
SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>

SPDX-License-Identifier: GPL-2.0-or-later
-->
<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/Logo-dark.svg">
    <img src="assets/Logo.svg" alt="Blabby" width="240">
  </picture>
</p>

# General Notes
It's just a hobby project, I only work on this project in my spare time.
The project started as a learning project to explore different development techniques and to experiment with new C++/Qt things.
But the overall goal is to have a cool working software somewhere in the future.

# Blabby
![Blabby CI](https://github.com/mFlorianW/blabby/workflows/Blabby%20CI/badge.svg)<br/>
Blabby is designed to control different UPNP speakers. 
The main purpose for this is to run on a Raspberry PI with a Display for controlling.

Still in a very very very early development phase. That means:

- Most of the stuff is not working ;-)
- Things that are implemented and should work, will have a high chance that they are have a tons of bugs. 

## Implemented Features
- Navigation of UPnPAV MediaServer
- Playback on UPnPAV MediaRenderer

# Runtime Requirements
Blabby uses the Roboto font, which is not bundled with the application.
The font must be installed on the system, e.g. with the package `fonts-roboto` on Debian based distributions
or `ttf-roboto` on Arch Linux.

# Build
Blabby uses CMakePresets for building. 
The build presets have the same as the configure presets.

The following CMakePresets are defined:

- debug
- debug-clang
- github-ci-debug
- release
- debug-sccache
- debug-clang-sccache
- release-sccache

The "-sccache" presets match their base preset but cache the compiler output with [sccache](https://github.com/mozilla/sccache),
which must be installed.
They build into their own directories (build-sccache, build-clang-sccache, build-release-sccache).

The workflow presets run the configure and build steps in one command:

- debug: configures and builds the "debug" preset
- release: configures and builds the "release" preset
- debug-sccache, release-sccache: the same with the "-sccache" presets

```sh
cmake --workflow --preset debug
```

## Pre-commit
Blabby has some pre-commit hooks for handling the licenses and formatting
The pre-commits can enabled by the CMake option with "ENABLE_PRECOMMIT=ON" or just use the "debug" preset.
The pre-commits feature needs a Python and pip environment.
The pip environment is only needed for the automatically install of the pre-commit Python module.

# Documentation
The detailed documentation of the Project can be found [here](https://mflorianw.github.io/blabby)

