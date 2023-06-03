# PhysiXal ![License](/resources/images/badge/license_unlicensed.svg)

"TO DO" --Put logo here--

PhysiXal is modern game engine in the early stages of development that makes possible of high fidelity 3D games and scenes. PhysiXal is in active development, new features are added constantly.

***


### Main features

- TBD


### Supported platforms

- Computer OS:
  - ![Windows supported](/resources/images/badge/windows_supported.svg)
  - ![Linux not supported](/resources/images/badge/linux_not_supported.svg)
  - ![MacOS not supported](/resources/images/badge/mac_not_supported.svg)
- Mobile OS:
  - ![Android not supported](/resources/images/badge/android_not_supported.svg)
  - ![IOS not supported](/resources/images/badge/ios_not_supported.svg)

Windows support only with plans for MacOS, Linux, Android and iOS support in the future.


### Hardware requirements
	
Vulkan API support for rendering only at this time. Vulkan SDK version 1.3. is needed as a minimum. The SPIR-V intermediate language for shaders is used.

***


## Getting setup
Visual Studio 2022 is recommended, PhysiXal is officially untested on other development environments whilst we focus on a Windows build.

<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/johantutak/PhysiXal`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

PhysiXal uses [Premake 5](https://premake.github.io/download) as a build generation tool.

<ins>**2. Configuring the dependencies:**</ins>

1. Run the [setup.bat](https://github.com/johantutak/PhysiXal/blob/master/scripts/setup.bat) file found in `scripts` folder. This will download the required prerequisites for the project if they are not present yet. A [Python](https://www.python.org/downloads) version of of 3.3 as a minimum is required to run the associated scripts.
2. One prerequisite is the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home). If it is not installed, the script will execute the `VulkanSDK.exe` file, and will prompt the user to install the SDK.
3. After installation, run the [setup.bat](https://github.com/johantutak/PhysiXal/blob/master/scripts/setup.bat) file again. If the Vulkan SDK is installed properly.
4. After downloading and unzipping the files, the [win_gen_projects.bat](https://github.com/johantutak/PhysiXal/blob/master/scripts/win/win_gen_projects.bat) script file will get executed automatically, which will then generate a Visual Studio solution file for user's usage.

If changes are made, or if you want to regenerate project files, rerun the [win_gen_projects.bat](https://github.com/johantutak/PhysiXal/blob/master/scripts/win/win_gen_projects.bat) script file found in `scripts/win` folder.

Delete the folders located at `engine\thirdparty\VulkanSDK` and `scripts\__pycache__`. As well as all files in `vendor\premake\bin` except for `premake5.exe` and `LICENSE.txt`.