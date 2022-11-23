workspace "MachineY"
    startproject "Engine"
    architecture "x64"

    configurations {
        "Debug" ,
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- IDK what these are for but here they are
mainpaths = {}
mainpaths["MachyDev"] = "C:\\Yock/Projects/GitHub/YEngine/MachyDev"
mainpaths["Engine"] = "C:\\Yock/Projects/GitHub/YEngine/Engine"

-- External Dependencies
externals = {}
externals["sdl2"] = "C:\\Yock/Extern/SDL2-2.24.2"
externals["imgui"] = "C:\\Yock/Extern/imgui-docking"
externals["entt"] = "C:\\Yock/Extern/entt-3.11.0"
externals["spdlog"] = "C:\\Yock/Extern/spdlog-1.11.0"
externals["glad"] = "external/glad"
externals["glm"] = "c:\\Yock/Extern/glm-master"

-- Glad before all
include "external/glad"

--------------------
-- Engine Library --
--------------------
project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files {
        "%{prj.name}/include/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{externals.imgui}/*.h",
        "%{externals.imgui}/*.cpp",
        "%{externals.glad}/src/**.c",
        "%{externals.glad}/include/**.h",
        "%{externals.glm}/**.hpp"
    }

    externalincludedirs {
        "%{prj.name}/include",
        "%{externals.sdl2}/include",
        "%{externals.imgui}",
        "%{externals.entt}/single_include",
        "%{externals.spdlog}/include",
        "%{externals.glad}/include",
        "%{externals.glm}"
    }

    flags { "FatalWarnings" }

    defines {
        "GLFW_INCLUDE_NONE" -- Ensures glad doesnt include glfw
    }

    filter { "system:windows" , "configurations:*" }
        systemversion "latest"
        defines {
            "MACHY_PLATFORM_WINDOWS",
            "MACHY_GUI"
        }

        libdirs {
            "%{externals.sdl2}/lib/x64"
        }

        links {
            "SDL2",
            "glad"
        }

    filter { "system:macosx" , "configurations:*" }
        systemversion "latest"
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines {
            "MACHY_PLATFORM_MAC"
        }

    filter { "system:linux" , "configurations:*" }
        defines {
            "MACHY_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines {
            "MACHY_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines {
            "MACHY_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

---------------------------
-- Machine Y Development --
---------------------------
project "MachyDev"
    location "MachyDev"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    links "Engine"

    targetdir(tdir)
    objdir(odir)

    files {
        "%{prj.name}/src/**.cpp",
        "%{externals.imgui}/*.h",
        "%{externals.imgui}/*.cpp",
        "%{externals.glm}/**.hpp"
    }

    externalincludedirs {
        "Engine/include",
        "%{externals.sdl2}/include",
        "%{externals.imgui}",
        "%{externals.spdlog}/include",
        "%{externals.entt}/single_include",
        "%{externals.glm}"
    }

    flags { "FatalWarnings" }

    filter { "system:windows" , "configurations:*" }
        systemversion "latest"
        defines {
            "MACHY_PLATFORM_WINDOWS",
            "MACHY_GUI"
        }

        libdirs {
            "%{externals.sdl2}/lib/x64"
        }

        links {
            "Engine",
            "SDL2",
            "glad"
        }

    filter { "system:macosx" , "configurations:*" }
        systemversion "latest"
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines {
            "MACHY_PLATFORM_MAC"
        }
        links {
            "SDL2.framework",
            "glad"
        }

    filter { "system:linux" , "configurations:*" }
        defines {
            "MACHY_PLATFORM_LINUX"
        }
        links {
            "SDL2",
            "glad"
        }

    filter "configurations:Debug"
        defines {
            "MACHY_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines {
            "MACHY_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

-----------
-- Pong  --
-----------
project "PongV1"
    location "PongV1"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    links "Engine"

    targetdir(tdir)
    objdir(odir)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{externals.imgui}/*.h",
        "%{externals.imgui}/*.cpp",
        "%{externals.glm}/**.hpp"
    }

    externalincludedirs {
        "%{prj.name}/GameObjects",
        "Engine/include",
        "%{externals.sdl2}/include",
        "%{externals.imgui}",
        "%{externals.spdlog}/include",
        "%{externals.entt}/single_include",
        "%{externals.glm}"
    }

    flags { "FatalWarnings" }

    filter { "system:windows" , "configurations:*" }
        systemversion "latest"
        defines {
            "MACHY_PLATFORM_WINDOWS",
            "MACHY_GUI"
        }

        libdirs {
            "%{externals.sdl2}/lib/x64"
        }

        links {
            "Engine",
            "SDL2",
            "glad"
        }

    filter { "system:macosx" , "configurations:*" }
        systemversion "latest"
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines {
            "MACHY_PLATFORM_MAC"
        }
        links {
            "SDL2.framework",
            "glad"
        }

    filter { "system:linux" , "configurations:*" }
        defines {
            "MACHY_PLATFORM_LINUX"
        }
        links {
            "SDL2",
            "glad"
        }

    filter "configurations:Debug"
        defines {
            "MACHY_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines {
            "MACHY_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"