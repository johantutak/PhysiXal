workspace "ThePhysiXalEngine"
	architecture "x86_64"
	startproject "Example"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories reletive to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "PhysiXal/thirdparty/GLFW/include"
IncludeDir["Vulkan"] = "PhysiXal/thirdparty/Vulkan/include"
IncludeDir["ImGui"] = "PhysiXal/thirdparty/imgui"
IncludeDir["glm"] = "PhysiXal/thirdparty/glm"

LibraryDir = {}
LibraryDir["Vulkan"] = "PhysiXal/thirdparty/Vulkan/lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.Vulkan}/vulkan-1.lib"

group "Dependencies"
	include "PhysiXal/thirdparty/GLFW"
	include "PhysiXal/thirdparty/imgui"

group ""

project "PhysiXal"
	location "PhysiXal"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "px_pch.h"
	pchsource "PhysiXal/src/px_pch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/thirdparty/glm/glm/**.hpp",
		"%{prj.name}/thirdparty/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/thirdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{prj.name}/thirdparty/Vulkan/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		filter "configurations:Debug"
			defines "PX_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "PX_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "PX_DIST"
			runtime "Release"
			optimize "on"

project "Example"
	location "Example"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"PhysiXal/thirdparty/spdlog/include",
		"PhysiXal/src",
		"PhysiXal/thirdparty",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"PhysiXal"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "PX_DEBUG"
		runtime "Debug"
		symbols "on"
	
		filter "configurations:Release"
		defines "PX_RELEASE"
		runtime "Release"
		optimize "on"
	
		filter "configurations:Dist"
		defines "PX_DIST"
		runtime "Release"
		optimize "on"