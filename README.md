# Metroid Prime Door Randomizer (MPDR)

This program allows you to change the types of doors in Metroid Prime randomly.

This is the Qt version of MPDR, which will eventually replace the previous version working in wxWidgets.

> **NOTE:** From version 0.3 onward, the wxWidgets repository will be deleted and be replaced with this repository (mpdr-qt), which will be renamed to mpdr.

## Supported platforms

Currently, MPDR supports the following **64-bit** systems:

* **Windows 7** and newer.
* **mac OS High Sierra (10.13)** and newer.
* **Linux:** Most distributions released after Ubuntu 16.04 LTS (Xenial), including rolling release distros like Arch.

## Usage

MPDR takes a disc file (ISO/GCM only) of Metroid Prime NTSC 0-00 or NTSC 0-02 version of the game,
and modifies the types of the doors accordingly. Currently it supports switching
to these types:

* **Blue** (all weapons)
* **Purple** (Wave Beam only)
* **White** (Ice Beam only)
* **Red** (Plasma Beam only)

> **NOTE:** Doors from Pirate Frigate, Impact Crater, and reactor doors from the Crashed Frigate will not be randomized. Currently, doors placed on the ground or the ceiling can't be randomized either.

The amount of doors of any given color can be adjusted with **door weights**.
These adjust the chance of any door to be that particular color, and are given in percentages, grouped by areas (Tallon Overworld, Chozo Ruins, Magmoor Caverns, Phendrana Drifts and Phazon Mines).

Doors can also be excluded from the randomization process, having the same color as they do in the original game.

You can save your configuration with JSON presets.

## Q & A

* **Q: Can I use this with the [Metroid Prime Item Radomizer](https://github.com/etaylor8086/metroid-prime-randomizer)?**
  * A: No, but it's possible that I might combine both randomizers in a single package. However, some versions might be able to use already modified disc images temporarily.
* **Q: But can I use the save files from the Item Randomizer?**
  * A: Yes, although this might be subject to change in the future.
* **Q: Are all configurations clearable?**
  * A: In theory, yes, so long as the Blue door weight remains high enough.
* **Q: Will you support Metroid Prime 2?**
  * A: No.

## Translation

> **NOTE:** It is heavily recommended that you use the source code for the latest "translator friendly" release to translate the program. You can find these on the Releases tab, and checking the Tags version.

### User Interface

The Qt version of MPDR can be localized using the same guidelines from the [Qt Linguist manual](https://doc.qt.io/qt-5/qtlinguist-index.html).

MPDR will automatically determine which language to use from the system's current locale. In the future, an option to switch languages may be added.

> **NOTE:** If a language is missing support or is unfinished, the missing translations will fallback to English (United States).

To add a new language, you have two options:

* **Generate a fresh translation file by [building the project](https://github.com/YonicDev/mpdr-qt#build).** You'll have to modify the `CMakeLists.txt` before building the program. The file itself has guidelines on how to do it.
* **Copy one of the existing translation files.** This is only recommended to do in Release and Translator Friendly builds.

> **NOTE:** The logs produced by the backend ([randomprime](https://github.com/YonicDev/randomprime), the actual randomizer) cannot be translated.

If you have an issue with translation such as requesting a disambiguation, [submit an issue](https://github.com/YonicDev/mpdr-qt/issues/new) with the translation label.

To update an existing translation file, follow the same steps as if you were to generate one, but use the same filename as the one that exists.

> **WARNING:** If you update the MPDR repository to a newer revision and you were required to rebuild the whole project (including randomprime), the translation files might be erased in the process! Hopefully, you can recover them by [reseting/discarding](https://git-scm.com/docs/git-reset) the change that causes them to be removed. *However, all local changes that weren't commited to the repository will be permanently lost.* So make sure you backup the files either way!

### Documentation

#### New translations

To translate the documentation regarding the latest version to a new language, you'll need to duplicate the following:

* The `doc_collection_en.qhcp` located in the `doc` folder. In this file, you'll have to remove all the doc entries of the previous versions unless you want to translate them as well.
* The `doc_en.qhp` located in the latest version folder in the `doc` directory.
* The `en` folder located in the base version folder and the latest revision folder in the version directory.

The `en` in the the files' names and the `en` folder name must be replaced with the language code specified in the [ISO 639-1 list of codes](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) that matches the language you want to translate to.

Afterwards, you can simply translate the html documents within the folder you've just duplicated. When you're done, modify the references to the files in both qhcp files. Then, you'll have to [build the project](https://github.com/YonicDev/mpdr-qt#build) to apply the new documentation.

The program should automatically be able to load the documentation set that matches the current system locale settings.

For more information on the help files, check [The Qt Help Framework](https://doc.qt.io/qt-5/qthelp-framework.html) documentation.

> **NOTE:** To apply all changes to the documentation, you will have to clear the CMake configuration, then rerun CMake, and finally build the project.

#### Updating translations

Version numbers are indicated by the format `v{major version}.{minor version}.{revision}`.

If you are adding a new version to an existing translation regardless of major or minor, you'll have to update the `<currentFilter>` and `<namespace>` fields in the collection qhcp file, and the `<filterAttribute>` and the `<customFilter name="">` entries in the ***new version***'s documentation qhp file to the target version. Otherwise, the new documentation will not be displayed properly.

On the other hand, if you are adding a new revision, you'll have to duplicate the contents of the `en` folder inside the revision folder (which is inside the version directory) and expand the qhp file that corresponds to the target version so that it incorporates the files on that new folder.

The keywords should be updated to link to the newer articles as well!

> **NOTE:** MPDR will fallback to the English documentation if the latest version's translated documentations are not available. Presently, this behavior can be disabled by setting the `FALLBACK_DOCUMENTATION` flag in docdialog.h to `0` at build time, but this can cause some functions of the documentation viewer to stop working correctly.

## Build

The following sections are meant for those who are interested in building the program from the source code rather than downloading it.

Although MPDR has been designed to be cross-platform, the setup and build configurations have only been tested for Windows and Linux (Ubuntu).

### Requirements

* **[CMake](https://cmake.org/download/) 3.5** or higher.
* **[Qt 5.13.2](https://www.qt.io/download)**, recommended with Qt Creator and Qt Docs as it includes the Clang compiler.
  * If you're not using the Clang compiler, you'll need the **MSVC** compiler included in [Visual Studio 2017 or 2019](https://visualstudio.microsoft.com/) in Windows, or the **[GCC 7.4](https://gcc.gnu.org/)** compiler in Linux.
  > **(mac OS only):** The Clang compiler provided by Qt is too old in Qt 5.13.2, so you must install **[Xcode 9.3](https://developer.apple.com/xcode/)** or newer. The Command Developer Tools will work just fine.
* **The [Rust](https://www.rust-lang.org/install.html) language:** Install the `nightly` toolchain alongside the `powerpc-unknown-linux-gnu` target, as indicated [here](https://github.com/YonicDev/randomprime/blob/master/compile_to_ppc/README.md).
  * If you've downloaded using rustup, you can use this command to install the correct target and toolchain:
  ```sh
  $ rustup-init -y --profile default --default-toolchain nightly --target powerpc-unknown-linux-gnu
  ```
  > **(Windows only)**, you might have to install the `x86_64-pc-windows-msvc` target as well. Simply add it to the --target parameter alongside the other.

### Procedure

1. Run the following command in your command line to clone the repository:

  ```sh
  $ git clone --recurse-submodules "https://github.com/YonicDev/mpdr"
  ```

2. After that, go to the randomprime submodule and run `cargo build`. Due to an issue with the compilation procedure, this extra step must be taken.

3. Once it's been compiled, return to the mpdr-qt folder and open the `CMakeLists.txt` file in Qt Creator to create the project. After configuring the project you simply have to hit the Build & Run button.

  * If you are prompted to select kits, you may select Clang/LLVM, an OS-specific kit (MSVC for Windows, and GCC for macOS and Linux), or both.

    The compiler in the kit you choose must support C++17. Here is a list of supported compiler versions:

    |OS     | MSVC               | GCC       | Clang     |
    |:-----:| :-----------------:| :--------:| :--------:|
    |Windows| Visual Studio 2017 | -         | 7         |
    |mac OS | -                  | Xcode 9.3 | Xcode 9.3 |
    |Linux  | -                  | 7.4       | 7         |

You may choose either Debug or Release configuration. The Minimum Size Release configuration is used to deploy prebuilt releases.

### Updating the repository

You can simply update the changes in the repository by performing a git pull. Sometimes it won't compile correctly after updating. Perform these steps from Qt one at a time and try compiling again. If it doesn't build, perform the next step:

1. Run CMake again.
2. Clean all CMake configuration.
3. Rebuild all projects.
4. Install the OS-specific kit, delete the CMakeLists.txt.user file in the repository folder, reopen the project and select the OS-specific kit.
5. Delete the build folders.
6. Delete the repository and clone it again.

If neither of these steps has worked, [submit an issue](https://github.com/YonicDev/mpdr-qt/issues/new) indicating your operating system and configuration, as well as all the steps you've made for configuring the whole thing.
