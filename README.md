# PhysiXal ![License](/resources/images/badge/License-Unlicensed.svg)

"TO DO" --Put logo here--

PhysiXal is modern game engine in the early stages of development that makes possible of high fidelity 3D games and scenes. PhysiXal is in active development, new features are added constantly.

***


### Main features

- TBD


### Supported platforms

- Computer OS:
  - ![Windows supported](/resources/images/badge/Windows-win-64.svg)
  - ![Linux not supported](/resources/images/badge/Linux-Not-Supported.svg)
  - ![MacOS not supported](/resources/images/badge/MacOS-Not-Supported.svg)
- Mobile OS:
  - ![Android not supported](/resources/images/badge/Android-Not-Supported.svg)
  - ![IOS not supported](/resources/images/badge/IOS-Not-Supported.svg)

Windows only support with plans for MacOS, Linux and Android and IOS support in the future.


### Hardware requirements
	
Vulkan API support for rendering only at this time. Vulkan SDK version 1.3.204 is needed as a minimum. The SPIR-V intermediate language for shaders is used.



## Installing and setup


Start by cloning the repository with `git clone --recursive https://github.com/johantutak/PhysiXal`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules. PhysiXal uses Premake 5 as a build generation tool.

Create a new directory called vendor and a other within it called bin.

Download [Premake](https://premake.github.io/download) from the website for your system.

Follow the steps relevant to your operating system.


## ![Windows](/resources/images/badge/Windows-win-64.svg)

Extract the downloaded file and put it in `vendor/bin` and create new directory called `premake`.
Extract the file and only put `premake5.exe` and `LISENSE.txt` in `vendor/bin/premake` since it's the only files needed.

PhysiXal is configured with:

Premake which generates project files for Visual Studio, Visual Studio 2022 is recommended. To generate the `.sln` and `.vcxproj` files for Visual Studio 2022, run `premake vs2022` at the command line. Or you may run `win_gen_projects.bat` as a convenience batch file for this task. Which is located in `scripts/win`.

Download and install the latest [Vulkan](https://vulkan.lunarg.com) SDK version from LunarG's website.
