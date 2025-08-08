workspace "Botapica"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Botapica"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "lib"
    }

    libdirs
    {
        "lib"
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
        links { "pthread" }

    filter "system:macosx"
        defines "PLATFORM_MACOS"