workspace "Botapica"
        architecture "x64"
        configurations { "Debug", "Release" }
        startproject "Botapica"

        outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GLFW"
        location "lib/glfw"
        kind "StaticLib"
        language "C"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
                "lib/glfw/src/context.c",
                "lib/glfw/src/init.c",
                "lib/glfw/src/input.c",
                "lib/glfw/src/monitor.c",
                "lib/glfw/src/platform.c",
                "lib/glfw/src/vulkan.c",
                "lib/glfw/src/window.c",
                "lib/glfw/src/null_init.c",
                "lib/glfw/src/null_monitor.c",
                "lib/glfw/src/null_window.c",
                "lib/glfw/src/null_joystick.c"
        }

        includedirs
        {
                "lib/glfw/include"
        }

        filter "system:linux"
                pic "on"
                systemversion "latest"
                staticruntime "on"

                files
                {
                        "lib/glfw/src/x11_init.c",
                        "lib/glfw/src/x11_monitor.c",
                        "lib/glfw/src/x11_window.c",
                        "lib/glfw/src/xkb_unicode.c",
                        "lib/glfw/src/posix_time.c",
                        "lib/glfw/src/posix_thread.c",
                        "lib/glfw/src/posix_module.c",
                        "lib/glfw/src/posix_poll.c",
                        "lib/glfw/src/glx_context.c",
                        "lib/glfw/src/egl_context.c",
                        "lib/glfw/src/osmesa_context.c",
                        "lib/glfw/src/linux_joystick.c"
                }

                defines
                {
                        "_GLFW_X11"
                }

                links
                {
                        "X11",
                        "Xcursor",
                        "Xinerama",
                        "Xrandr",
                        "pthread",
                        "dl"
                }

        filter "system:windows"
                systemversion "latest"
                staticruntime "on"

                files
                {
                        "lib/glfw/src/win32_init.c",
                        "lib/glfw/src/win32_joystick.c",
                        "lib/glfw/src/win32_monitor.c",
                        "lib/glfw/src/win32_time.c",
                        "lib/glfw/src/win32_thread.c",
                        "lib/glfw/src/win32_window.c",
                        "lib/glfw/src/wgl_context.c",
                        "lib/glfw/src/egl_context.c",
                        "lib/glfw/src/osmesa_context.c"
                }

                defines
                {
                        "_GLFW_WIN32",
                        "_CRT_SECURE_NO_WARNINGS"
                }

        filter "system:macosx"
                systemversion "10.15"
                staticruntime "on"

                files
                {
                        "lib/glfw/src/cocoa_init.m",
                        "lib/glfw/src/cocoa_joystick.m",
                        "lib/glfw/src/cocoa_monitor.m",
                        "lib/glfw/src/cocoa_window.m",
                        "lib/glfw/src/cocoa_time.c",
                        "lib/glfw/src/posix_thread.c",
                        "lib/glfw/src/posix_module.c",
                        "lib/glfw/src/nsgl_context.m",
                        "lib/glfw/src/egl_context.c",
                        "lib/glfw/src/osmesa_context.c"
                }

                defines
                {
                        "_GLFW_COCOA"
                }


        filter "configurations:Debug"
                runtime "Debug"
                symbols "on"
                optimize "off"

        filter "configurations:Release"
                runtime "Release"
                symbols "on"
                optimize "on"

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
                "lib/imgui/imgui_widgets.cpp",
                "lib/imgui/backends/imgui_impl_glfw.cpp",
                "lib/imgui/backends/imgui_impl_opengl3.cpp"
        }

        includedirs
        {
                "lib/imgui",
                "lib/imgui/backends",
                "lib/glfw/include"
        }

        filter "system:windows"
                includedirs { 
                        "lib/winglew/include",
                        "lib/winglfw/include"
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
                "Source/**/*.cpp",
		"Include/**.h"
	}

        includedirs
        {
                "Include",
                "lib/glfw/include",
                "lib/imgui",
                "lib/imgui/backends",
                "lib/glm"
        }

        links
        {
                "ImGui"
        }

        filter "system:not windows"
                links { "GLFW" }

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
                defines { "PLATFORM_WINDOWS", "GLEW_STATIC" }
                includedirs { 
                        "lib/winglew/include",
                        "lib/winglfw/include"
                }

        filter { "system:windows", "toolset:gcc" }
                libdirs { 
                        "lib/winglew/lib/Release/x64",
                        "lib/winglfw/lib-mingw-w64"
                }
                links { "glew32s", "glfw3", "opengl32", "gdi32", "user32", "kernel32" }

        filter { "system:windows", "toolset:msc*" }
                libdirs { 
                        "lib/winglew/lib/Release/x64",
                        "lib/winglfw/lib-vc2022"
                }
                links { "glew32s", "glfw3_mt", "opengl32", "gdi32", "user32", "kernel32" }

        filter "system:linux"
                defines "PLATFORM_LINUX"
                links {
                        "X11",
                        "Xcursor",
                        "Xinerama",
                        "Xrandr",
                        "pthread",
                        "GL",
                        "GLEW",
                        "dl"
                }

        filter "system:macosx"
                defines "PLATFORM_MACOS"
                links {
                        "GLEW",
                        "OpenGL.framework",
                        "Cocoa.framework",
                        "IOKit.framework",
                        "CoreVideo.framework",
                        "CoreFoundation.framework",
                        "Foundation.framework",
                        "QuartzCore.framework"
                }
                libdirs { "/opt/homebrew/lib" }
                includedirs { "/opt/homebrew/include" }

        filter { "system:macosx", "configurations:Debug" }
                buildoptions { "-fsanitize=address" }
                linkoptions { "-fsanitize=address" }