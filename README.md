# Metroid Prime Door Randomizer (MPDR)

This program allows you to change the types of doors in Metroid Prime randomly.

This is the Qt version of MPDR, which will eventually replace the previous version working in wxWidgets.

## Supported platforms

Currently, MPDR supports the following **64-bit** systems:

* **Windows 7** and newer.
* **Linux:** Tested on Arch and Ubuntu-based distributions.

MPDR is also designed to work with Mac OS in the future.

## Usage

MPDR takes a disc file (ISO/GCM only) of Metroid Prime NTSC 0-00 or NTSC 0-02 version of the game,
and modifies the types of the doors accordingly. Currently it supports switching
to these types:

* **Blue** (all weapons)
* **Purple** (Wave Beam only)
* **White** (Ice Beam only)
* **Red** (Plasma Beam only)

> **NOTE:** Doors from Pirate Frigate, Impact Crater, and reactor doors from the Crashed Frigate will not be randomized. Currently, vertical doors (placed on the ground or the ceiling), and doors from the Crashed Frigate's ventilation system can't be randomized either.

The amount of doors of any given color can be adjusted with **door weights**.
These adjust the chance of any door to be that particular color, and are given in percentages, grouped by areas (Tallon Overworld, Chozo Ruins, Magmoor Caverns, Phendrana Drifts and Phazon Mines).

You can save your configuration with JSON presets.

## Q & A

* **Q: Can I use this with the [Metroid Prime Item Radomizer](https://github.com/etaylor8086/metroid-prime-randomizer)?**
  * A: No, but it's possible that I might combine both randomizers in a single package.
* **Q: But can I use the save files from the Item Randomizer?**
  * A: Yes, although this might be subject to change in the future.
* **Q: Are all configurations clearable?**
  * A: In theory, yes, so long as the Blue door weight remains high enough.
* **Q: Will you support Metroid Prime 2?**
  * A: Not at this moment.

## Translation

The Qt version of MPDR can be localized using the same guidelines from the [Qt Linguist manual](https://doc.qt.io/qt-5/qtlinguist-index.html).

MPDR will automatically determine which language to use from the system's current locale. In the future, an option to switch languages may be added.

> **NOTE:** If a language is missing support or is unfinished, the missing translations will fallback to English (United States).

To add a new language, you have two options:

* **Generate a fresh translation file by [building the project](https://github.com/YonicDev/mpdr-qt#build).** You'll have to modify the `CMakeLists.txt` before building the program. The file itself has guidelines on how to do it.
* **Copy one of the existing translation files.** This is NOT recommended as some translation files might become updated as the program is developed.

> **NOTE:** The logs produced by the backend ([randomprime](https://github.com/YonicDev/randomprime), the actual randomizer) cannot be translated.

If you have an issue with translation such as requesting a disambiguation, [submit an issue](https://github.com/YonicDev/mpdr-qt/issues/new) with the translation label.

> **WARNING:** If you update the MPDR repository to a newer revision and you were required to rebuild the whole project (including randomprime), the translation files might be erased in the process! Hopefully, you can recover them by [reseting/discarding](https://git-scm.com/docs/git-reset) the change that causes them to be removed. *However, all local changes that weren't commited to the repository will be permanently lost.* So make sure you backup the files either way!

## Build

The following sections are meant for those who are interested in building the program from the source code rather than downloading it.

Although MPDR has been designed to be cross-platform, the setup and build configurations have only been tested for Windows and Linux (Ubuntu).

### Requirements

* **[CMake](https://cmake.org/download/) 3.5** or higher.
* **[Qt 5.13.2](https://www.qt.io/download)**, recommended with Qt Creator and Qt Docs as it includes the Clang compiler.
* **The [Rust](https://www.rust-lang.org/install.html) language:** Install the `nightly` toolchain alongside the `powerpc-unknown-linux-gnu` target, as indicated [here](https://github.com/YonicDev/randomprime/blob/master/compile_to_ppc/README.md).

### Procedure

Run the following command in your command line to clone the repository:

```sh
$ git clone --recurse-submodules "https://github.com/YonicDev/mpdr"
```

Then open the `CMakeLists.txt` file in Qt Creator to create the project. After configuring the project you simply have to hit the Build & Run button.

> If you are prompted to select kits, you may select Clang or MSVC (Windows) / GCC (Linux), or both.

You may choose either Debug or Release configuration.

### Updating the repository

You can simply update the changes in the repository by performing a git pull. Sometimes it won't compile correctly after updating. Perform these steps from Qt one at a time and try compiling again. If it doesn't build, perform the next step:

1. Run CMake again.
2. Clean all CMake configuration.
3. Rebuild all projects.
4. Delete the build folder.
5. Delete the repository and clone it again.

If neither of these steps has worked, [submit an issue](https://github.com/YonicDev/mpdr-qt/issues/new) indicating your operating system and configuration, as well as all the steps you've made for configuring the whole thing.
