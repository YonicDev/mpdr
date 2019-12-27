# Metroid Prime Door Randomizer (MPDR)

This program allows you to change the types of doors in Metroid Prime randomly.

## Usage

MPDR takes a disc file (ISO/GCM only) of Metroid Prime NTSC 0-00 version of the game,
and modifies the types of the doors accordingly. Currently it supports switching
to these types:

* **Blue** (all weapons)
* **Purple** (Wave Beam only)
* **White** (Ice Beam only)
* **Red** (Plasma Beam only)

**NOTE:** Although Metroid Prime NTSC 0-02 version is also supported, it has not been properly tested.

Doors from Pirate Frigate and Impact Crater will not be randomized.

The amount of doors of any given color can be adjusted with **door weights**.
These adjust the chance of any door to be that particular color, and are given in percentages, grouped by areas (Tallon Overworld, Chozo Ruins, Magmoor Caverns, Phendrana Drifts and Phazon Mines).

You can save your configuration of seed and door weights with JSON presets.

## Q & A

* **Q: Can I use this with the [Metroid Prime Item Radomizer](https://github.com/etaylor8086/metroid-prime-randomizer)?**
  * A: No, but it's possible that I might combine both randomizers in a single package.
* **Q: But can I use the save files from the Item Randomizer?**
  * A: Yes, although this might be subject to change in the future.
* **Q: What platforms does it support?**
  * A: Currently it supports Windows 10 64-bit, although it is also designed to work with Mac OS and 64-bit versions of Linux eventually.
* **Q: Are all configurations clearable?**
  * A: In theory, yes, so long as the Blue door weight remains high enough.
* **Q: Will you support Metroid Prime 2?**
  * A: Not at this moment.

***
The following sections are meant for those who are interested in building the program from the source code rather than downloading it.

## Build
Although MPDR has been designed to be cross-platform, the setup and build configurations have only been documented for Windows.

> **NOTE:** Automatization of this process with CMake is on the works. It is not guaranteed to build correctly as documentation is not fully implemented yet!

Currently only Windows 10 has been tested.

**NOTE:** 32-bit builds are supported, but non-functional.

### Requirements

* **[CMake](https://cmake.org/download/) 3.10** or higher.
* **Visual Studio 2017** or 2019 with C++ support and the **Windows SDK 10.0.14393.0** or newer.
* **The [Rust](https://www.rust-lang.org/install.html) language:** Install the `nightly` toolchain alongside the `powerpc-unknown-linux-gnu` target, as indicated [here](https://github.com/YonicDev/randomprime/blob/master/compile_to_ppc/README.md).
* **Libraries:** [wxWidgets](https://github.com/wxWidgets/wxWidgets) and the Door Randomizer fork of [randomprime](https://github.com/YonicDev/randomprime).

### CMake
Run the following commands in either cmd or PowerShell:

```posh
> mkdir mpdr
> cd mpdr
> git clone --recurse-submodules "https://github.com/YonicDev/mpdr"
> mkdir build
> cd build
> cmake .. -G "Visual Studio 15 2017 Win64"

```

Then open the generated MPDR solution in Visual Studio and simply Compile & Run. You may choose either Debug or Release configuration.

> For Debug configuration, please follow the manual configuration as it will fail to compile without some adjustments.

### Manual configuration

If the automatic configuration didn't compile, follow these instructions.

Apart from linking the libraries, the following configuration must be used:
* Import properties from the **wxWidgets** library.
> **NOTE:** Make sure to use the Release static libraries for Debug configuration!
* **Preprocessor definitions:** `_CRT_SECURE_NO_DEPRECATE` and `_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING`
* In Debug configuration, use **DLL Multiprocess (/MD)** as linking runtime libraries.
* **Additional libraries:** `ws2_32.lib` and `userenv.lib`.
