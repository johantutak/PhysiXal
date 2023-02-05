# PhysiXal Engine

"TO DO" --Put logo here--

## About

TBD.

***
## Main features

### Supported:

- TBD

### Planned:

- TBD

***

## PhysiXal Engine support

PhysiXal Engine is in active development. Here is a short list of what is supported and what isn't. This can change at any time.

### Supported platforms

Currently PhysiXal supports:

- Computer OS:
  - ![Windows supported](resources/images/badge/Windows win-64.svg)
  - ![Linux supported](https://img.shields.io/badge/Linux-Debian-green.svg)
  - ![MacOS not supported](https://img.shields.io/badge/MacOS-Not%20Supported-red.svg)
- Mobile OS:
  - ![Android not supported](https://img.shields.io/badge/Android-Not%20Supported-red.svg)
  - ![IOS not supported](https://img.shields.io/badge/IOS-Not%20Supported-red.svg)

Windows and Linux is currently supported with plans for MacOS and Android/IOS support in the future.


### Hardware requirements:
	
PhysiXal uses the Vulkan API for rendering only at this time. Vulkan SDK version 1.3.204 is needed as a minimum. The SPIR-V intermediate language for shaders is used.



## Installing and setup:


Start by cloning the repository with `git clone --recursive https://github.com/johantutak/ThePhysiXalEngine`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules. PhysiXal Engine uses Premake 5 as a build generation tool.

Create a new directory called vendor and a other within it called bin.

Download [Premake](https://premake.github.io/download.html) from the website for your system.

Follow the steps relevant to your operating system.


![Windows](https://img.shields.io/badge/Windows-win--64-green.svg):

Extract the downloaded file and put it in `vendor/bin` and create new directory called `premake`.
Extract the file and only put `premake5.exe` and `LISENSE.txt` in `vendor/bin/premake` since it's the only files needed.

Change the file name of `premake5.lua.win.txt` to `premake5.lua`.

PhysiXal is configured with:

Premake generates project files for Visual Studio, Visual Studio 2022 is recommended. To generate the `.sln` and `.vcxproj` files for Visual Studio 2022, run `premake vs2022` at the command line. Or you may run `win_gen_projects.bat` as a convenience batch file for this task. Which is located in `scripts/win`.

PhysiXal uses the Vulkan API and needs to be installed.

Download the latest [Vulkan](https://vulkan.lunarg.com) SDK version from LunarG's website.

Install the SDK.


![Linux](https://img.shields.io/badge/Linux-Debian-green.svg):

Extract the downloaded file and put it in `vendor/bin` and create new directory called `premake`. 
Extract the file and only put all files in `vendor/bin/premake`.

Change the file name of `premake5.lua.lnx.txt` to `premake5.lua`.

PhysiXal is configured with:

Premake generates `Makefiles` files for C++ IDE's. To generate the Makefile for your IDE, run `premake5 gmake2` at the terminal in solution directory. Or you may run `lnx_gen_projects.sh` as a convenience bash file for this task.

Make `lnx_gen_projects.sh` executable by navigating to `ThePhysiXalEngine/scripts/lnx` on the terminal using `cd`. then run `chmod u+x lnx_gen_projects.sh`. Now ur able to run it by left clicking.

PhysiXal is compiled with:

The `make` command will compile the project for you and make it into a x executable file, run `make` at the terminal in the solution directory. Or you may run `lnx_build_debug.sh` as a convenience bash file for this task.

Make `lnx_build_debug.sh` executable by navigating to `ThePhysiXalEngine/scripts/lnx` on the terminal using `cd`. Then run `chmod u+x lnx_build_debug.sh`. Now ur able to run it by left clicking.

Debug configuration is only supported at the moment.

#### Only tested on ![Ubuntu 22.04 LTS](https://img.shields.io/ubuntu/v/jellyfish) on wayland aswell as X11 (Xorg).

PhysiXal Engine uses a few Vulkan/GLFW API features and needs to be installed.

PhysiXal Engine has extra development dependencies needed for ![Linux](https://img.shields.io/badge/Linux-Debian-green.svg). The following packages are needed to compile the project:   
    
    libxrandr
    libxinerama
    libxi
    libxxf86vm
    glfw3
    libvulkan-dev
    vulkan-sdk

Install the additional dependencies by running:

`sudo apt install libxrandr-dev libxinerama-dev libxi-dev libxxf86vm-dev libglfw3-dev libglfw3 vulkan-tools libvulkan-dev vulkan-validationlayers-dev spirv-tools`

PhysiXal uses Google's `glslc shader compiler. Download the gcc "DEBUG" version of Google's unofficial [binaries](https://github.com/google/shaderc/blob/main/downloads.md) for Linux.

Copy `glslc` to your `/usr/local/bin`. Note you may need to `sudo cd` form extracted directory to target directory, depending on your permissions.

Navigate into `Example/assets/shaders` in terminal and use `chmod +x compile.sh` to make file executable by left clicking. Run it by left clicking when you need to complie into Spir-V bytecode.


#### NOTE!

"Before commiting a version of the software, change back the `premake5.lua` to `premake5.lua. "OS used" (lnx or win) .txt`."
