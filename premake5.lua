-- ###################################################################
--							  PhysiXal
-- ###################################################################

workspace "PhysiXal"
	architecture "x86_64"
	startproject "Editor"

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

-- Extract Vulkan to Development enviroment (install directory )
VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories reletive to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "engine/thirdparty/GLFW/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["ImGui"] = "engine/thirdparty/imgui"
IncludeDir["ImGuizmo"] = "engine/thirdparty/ImGuizmo"
IncludeDir["glm"] = "engine/thirdparty/glm"
IncludeDir["stb_image"] = "engine/thirdparty/stb_image"
IncludeDir["tinyobjloader"] = "engine/thirdparty/tinyobjloader"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

group "Dependencies"
	include "engine/thirdparty/GLFW"
	include "engine/thirdparty/imgui"
group ""

-- ###################################################################
--					##########################
-- ###################################################################



-- ###################################################################
--							  Engine
-- ###################################################################

project "Engine"
	location "engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "px_pch.h"
	pchsource "engine/src/px_pch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/thirdparty/stb_image/**.h",
		"%{prj.name}/thirdparty/stb_image/**.cpp",
		"%{prj.name}/thirdparty/glm/glm/**.hpp",
		"%{prj.name}/thirdparty/glm/glm/**.inl",
		"%{prj.name}/thirdparty/tinyobjloader/**.h",
		"%{prj.name}/thirdparty/tinyobjloader/**.cpp",

		"%{prj.name}/thirdparty/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/thirdparty/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobjloader}"
	}

	links
	{
		"GLFW",
		"%{Library.Vulkan}",
		"ImGui"
	}

	flags { "NoPCH" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		removefiles { "src/core/platform/lnx/**.h",
		              "src/core/platform/lnx/**.cpp" }
		
		defines
		{
			"PX_PLATFORM_WINDOWS"
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

-- ###################################################################
--					##########################
-- ###################################################################



-- ###################################################################
--							  Editor
-- ###################################################################

project "Editor"
	location "editor"
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
		"engine/thirdparty/spdlog/include",
		"engine/src",
		"engine/thirdparty",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{
		"engine"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		links 
		{
		}

		defines 
    	{
    		"PX_PLATFORM_WINDOWS"
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

-- ###################################################################
--					##########################
-- ###################################################################