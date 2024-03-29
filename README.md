# Sound Bakery

[![linux](https://github.com/KarateKidzz/sound-bakery/actions/workflows/linux.yaml/badge.svg?branch=dev)](https://github.com/KarateKidzz/sound-bakery/actions/workflows/linux.yaml)
[![macos](https://github.com/KarateKidzz/sound-bakery/actions/workflows/macos.yaml/badge.svg?branch=dev)](https://github.com/KarateKidzz/sound-bakery/actions/workflows/macos.yaml)
[![windows](https://github.com/KarateKidzz/sound-bakery/actions/workflows/windows.yaml/badge.svg?branch=dev)](https://github.com/KarateKidzz/sound-bakery/actions/workflows/windows.yaml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![GitHub license](https://img.shields.io/badge/C%2B%2B-20-blue)](https://en.cppreference.com/w/cpp/compiler_support#cpp20)
![WIP](https://img.shields.io/badge/Status-WIP-yellow)

*Sound Bakery is currently under development!*

Sound Bakery is an open-source audio middleware tool with an editing workflow like Wwise and an API like FMOD. It aims to be a free alternative to the industry standards without sacrificing quality. It is also highly customisable and easy to edit.

It is built on [miniaudio](https://miniaud.io/index.html) for audio playback and [Dear ImGui](https://github.com/ocornut/imgui) for editing.

The library comes in three parts:
- A cross-platform editor
- An adaptive audio library called `Sound Bakery`
- An audio playback library called `Sound Chef` with features like virtual voices

# Features
- Cross-platform authoring application
- Compiles on Windows, MacOS, Linux GCC and Linux Clang
- Built with C++20
- Public C API
- Adaptive audio support with container types mirroring Wwise
- Soundbanks

# License

Sound Bakery is distributed under the MIT [license](https://github.com/KarateKidzz/sound-bakery/blob/master/LICENSE).

The library and application are free to use in personal and commercial applications.
