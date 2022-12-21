# PhysiXal Engine

PhysiXal Engine support:

PhysiXal is in active development. Here is a short list of what is supported and what isn't. This can change at any time.


Supported platforms:

Currently PhysiXal supports:

    Computer OS:
        Windows supported
        Linux supported
        MacOS not supported
    Mobile OS:
        Android not supported
        IOS not supported

Windows and Linux is currently supported with plans for MacOS and Android/IOS support in the future.



Hardware requirements:

PhysiXal uses: 
	Vulkan Rendering for Windows.
	OpenGL Rendering for Linux.



Installing and setup:


Start by cloning the repository with git clone --recursive https://github.com/johantutak/ThePhysiXalEngine.

If the repository was cloned non-recursively previously, use git submodule update --init to clone the necessary submodules. PhysiXal uses Premake 5 as a build generation tool.

Create a new directory called vendor and a other within it called bin.

Download Premake5 from https://premake.github.io/download for your system.

Next: Follow the steps relevant to your operating system.


Windows:

Extract the downloaded file and put it in vendor/bin and create new directory called premake.
Extract the file and only put premake5.exe and LISENSE.txt in vendor/bin/premake since it's the only files needed.

Change the file name of premake5.lua.win.txt to premake5.lua.

Premake generates project files for Visual Studio, Visual Studio 2022 is recommended. To generate the .sln and .vcxproj files for Visual Studio 2022, run premake vs2022 at the command line. Or you may run win_gen_projects.bat as a convenience batch file for this task.

PhysiXal uses the Vulkan API and needs to be installed.

Download the Vulkan SDK version 1.3.236 from https://vulkan.lunarg.com

Install the SDK.


Linux:

Extract the downloaded file and put it in vendor/bin and create new directory called premake. 
Extract the file and only put all files in vendor/bin/premake.

Change the file name of premake5.lua.lnx.txt to premake5.lua.

PhysiXal is configured with:

Premake generates Makefiles files for C++ IDE's, Apache Netbeans is recommended. To generate the Makefile for your IDE, run $ premake5 gmake2 at the command line in solution directory. Or you may run lnx_gen_projects.sh as a convenience bash file for this task.

Make lnx_gen_projects.sh executable by navigating to ThePhysiXalEngine/scripts on the command line using $ cd. then run chmod u+x lnx_gen_projects.sh. now ur able to run it by left clicking.

PhysiXal is compiled with:

The Make command will compile the project for you and make it into a x executable file, run $ make at the command line in solution directory. Or you may run lnx_build_debug.sh as a convenience bash file for this task.

Make lnx_build_debug.sh executable by navigating to ThePhysiXalEngine/scripts on the command line using $ cd. then run chmod u+x lnx_build_debug.sh. now ur able to run it by left clicking.

Debug configuration is only supported at the moment.

Debian (Ubuntu) X11 support only. Further support later on.

PhysiXal uses a few OpenGL/GLFW API features and needs to be installed. Installing by Tarball to a custom directory is not supported by this time.

PhysiXal has extra development dependencies needed for Linux. The following packages are needed to compile the project:   
    
    libxcursor
    libxrandr
    libxinerama
    libxi
    zenity
    glfw3

Install the additional dependencies by running:

$ sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglu1-mesa-dev libglfw3-dev libglfw3


NOTE!:
	Before commiting a version of the software, change back the premake5.lua to premake5.lua. OS used (lnx or win) .txt.

