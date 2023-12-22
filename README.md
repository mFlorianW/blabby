<!--
SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>

SPDX-License-Identifier: GPL-2.0-or-later
-->
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

# Build
Blabby uses CMakePresets for building. 
The build presets have the same as the configure presets.

The following CMakePresets are defined:

- debug
- debug-clang
- github-ci-debug
- release


# Documentation
The detailed documentation of the Project can be found [here](https://mflorianw.github.io/blabby)

