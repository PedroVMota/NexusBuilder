workspace "Botapica"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Botapica"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GLEW"
	location "lib/glew"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"lib/glew/src/glew.c"
	}

	includedirs
	{
		"lib/glew/include"
	}

	defines
	{
		"GLEW_STATIC"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "system:windows"
		systemversion "latest"
		links { "opengl32" }

	filter "system:linux"
		links { "GL" }

	filter "system:macosx"
		links { "OpenGL.framework" }

project "ImGui"
	location "lib/imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"lib/imgui/imgui.cpp",
		"lib/imgui/imgui_demo.cpp",
		"lib/imgui/imgui_draw.cpp",
		"lib/imgui/imgui_tables.cpp",
		"lib/imgui/imgui_widgets.cpp"
	}

	includedirs
	{
		"lib/imgui"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "system:windows"
		systemversion "latest"

project "Botapica"
	location "."
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	targetname "Botapica"

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Include/**.h"
	}

	includedirs
	{
		"Include",
		"lib/glew/include",
		"lib/imgui"
	}

	links
	{
		"GLEW",
		"ImGui"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"

	filter "system:linux"
		defines "PLATFORM_LINUX"
		links { "pthread", "GL" } -- Added GL here for final linking

	filter "system:macosx"
		defines "PLATFORM_MACOS"