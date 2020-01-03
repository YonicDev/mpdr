# Metroid Prime Door Randomizer (MPDR)

This program allows you to change the types of doors in Metroid Prime randomly.

This is the Qt version of MPDR, which will eventually replace the previous version working in wxWidgets.

## Usage

MPDR takes a disc file (ISO/GCM only) of Metroid Prime NTSC 0-00 version of the game,
and modifies the types of the doors accordingly. Currently it supports switching
to these types:

* **Blue** (all weapons)
* **Purple** (Wave Beam only)
* **White** (Ice Beam only)
* **Red** (Plasma Beam only)

> **NOTE 1:** Although Metroid Prime NTSC 0-02 version is also supported, it has not been properly tested.

> **NOTE 2:** Doors from Pirate Frigate and Impact Crater will not be randomized. Currently, vertical doors (placed on the ground or the ceiling) can't be randomized either.

The amount of doors of any given color can be adjusted with **door weights**.
These adjust the chance of any door to be that particular color, and are given in percentages, grouped by areas (Tallon Overworld, Chozo Ruins, Magmoor Caverns, Phendrana Drifts and Phazon Mines).

You can save your configuration with JSON presets.

## Q & A

* **Q: Can I use this with the [Metroid Prime Item Radomizer](https://github.com/etaylor8086/metroid-prime-randomizer)?**
  * A: No, but it's possible that I might combine both randomizers in a single package.
* **Q: But can I use the save files from the Item Randomizer?**
  * A: Yes, although this might be subject to change in the future.
* **Q: What platforms does it support?**
  * A: Currently it supports Windows 10 and Linux,  although it is also designed to work with Mac OS eventually. 32-bit versions are not supported.
* **Q: Are all configurations clearable?**
  * A: In theory, yes, so long as the Blue door weight remains high enough.
* **Q: Will you support Metroid Prime 2?**
  * A: Not at this moment.

## Translation

The Qt version of MPDR can be localized using the same guidelines from the [Qt Linguist manual](https://doc.qt.io/qt-5/qtlinguist-index.html).

MPDR will automatically determine which language to use from the system's current locale. In the future, an option to switch languages may be added.

> **WARNING:** If a language is missing support or is unfinished, the missing translations will fallback to English (United States).

To add a new language, you have two options:

* **Generate a fresh translation file by [building the project](https://github.com/YonicDev/mpdr-qt#build).** You'll have to modify the `CMakeLists.txt` before building the program. The file itself has guidelines on how to do it.
* **Copy one of the existing translation files.** This is NOT recommended as some translation files might become updated as the program is developed.

> **NOTE:** The logs produced by the backend ([randomprime](https://github.com/YonicDev/randomprime), the actual randomizer) cannot be translated.

If you have an issue with translation such as requesting an disambiguation, [submit an issue](https://github.com/YonicDev/mpdr-qt/issues/new) with the translation label.

## Build

The following sections are meant for those who are interested in building the program from the source code rather than downloading it.

Although MPDR has been designed to be cross-platform, the setup and build configurations have only been tested for Windows and Linux (Ubuntu).

### Requirements

* **[CMake](https://cmake.org/download/) 3.5** or higher.
* **[Qt 5](https://www.qt.io/download)**, recommended with Qt Creator.
* **The [Rust](https://www.rust-lang.org/install.html) language:** Install the `nightly` toolchain alongside the `powerpc-unknown-linux-gnu` target, as indicated [here](https://github.com/YonicDev/randomprime/blob/master/compile_to_ppc/README.md).

### Procedure

Run the following command in your command line to clone the repository:

```sh
$ git clone --recurse-submodules "https://github.com/YonicDev/mpdr"
```

Then open the `CMakeLists.txt` file in Qt Creator to create the project. After configuring the project you simply have to hit the Build & Run button.

> If you are prompted to select kits, you may select Clang or MSVC (Windows) / GCC (Linux), or both.

You may choose either Debug or Release configuration.
