workspace "MachineY"
    startproject "MachyDev"
    architecture "x64"

    configurations {
        "Debug" ,
        "Release"
    }

    tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
    odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

    -- External Dependencies
    externals = {}
    externals["sdl2"] = "external/SDL2-2.24.2"
    externals["imgui"] = "external/imgui-docking"
    externals["entt"] = "external/entt"
    externals["spdlog"] = "external/spdlog-1.11.0"
    externals["glad"] = "external/glad"
    externals["glm"] = "external/glm-master"
    externals["stb"] = "external/stb"
    externals["ImEntt"] = "external/ImEnttComponent"
    externals["json"] = "external/json"
    externals["boost"] = "external/boost"
    externals["imguizmo"] = "external/imguizmo"
    externals["box2d"] = "external/box2d"

    -- Glad before all
    include "external/glad"
    include "external/box2d"

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
            "%{externals.entt}/.hpp",
            "%{externals.glad}**.c",
            "%{externals.glad}**.h",
            "%{externals.glm}/**.hpp",
            "%{externals.stb}/*.h",
            "%{externals.ImEntt}/*.hpp",
            "%{externals.json}/**.hpp",
            "%{externals.imguizmo}/**.h",
            "%{externals.imguizmo}/**.cpp",
            "%{externals.box2d}/include/**.h",
            "%{externals.box2d}/src/**.cpp"
        }

        externalincludedirs {
            "%{prj.name}/include",
            "%{externals.sdl2}/include",
            "%{externals.imgui}",
            "%{externals.entt}",
            "%{externals.spdlog}/include",
            "%{externals.glad}/include",
            "%{externals.glm}",
            "%{externals.stb}",
            "%{externals.ImEntt}",
            "%{externals.json}/include",
            "%{externals.imguizmo}",
            "%{externals.box2d}/include"
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
                "glad",
                "box2d"
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
            "%{prj.name}/src/**.hpp"
        }

        externalincludedirs {
            "%{prj.name}/src",
            "%{prj.name}/core",
            "%{prj.name}/gui",
            "Engine/include",
            "%{externals.sdl2}/include",
            "%{externals.glad}/include",
            "%{externals.spdlog}/include",
            "%{externals.entt}",
            "%{externals.imgui}",
            "%{externals.glm}",
            "%{externals.stb}",
            "%{externals.ImEntt}",
            "%{externals.json}/include",
            "%{externals.imguizmo}",
            "%{externals.box2d}/include"
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
                "glad",
                "box2d"
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
                "Engine",
                "SDL2.framework",
                "glad",
                "box2d"
            }

        filter { "system:linux" , "configurations:*" }
            defines {
                "MACHY_PLATFORM_LINUX"
            }
            links {
                "Engine",
                "SDL2",
                "glad",
                "box2d"
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

    ---------------------
    -- DnD Development --
    ---------------------
    project "DnD"
        location "DnD"
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
            "gameScripts/*.hpp"
        }

        externalincludedirs {
            "%{prj.name}/src",
            "%{prj.name}/gameScripts",
            "Engine/include",
            "%{externals.sdl2}/include",
            "%{externals.glad}/include",
            "%{externals.spdlog}/include",
            "%{externals.entt}",
            "%{externals.imgui}",
            "%{externals.glm}",
            "%{externals.stb}",
            "%{externals.ImEntt}",
            "%{externals.json}/include",
            "%{externals.imguizmo}",
            "%{externals.box2d}/include",
            "DnD/gameScripts",
            "DnD/gameObjects"
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
                "glad",
                "box2d"
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
                "Engine",
                "SDL2.framework",
                "glad",
                "box2d"
            }

        filter { "system:linux" , "configurations:*" }
            defines {
                "MACHY_PLATFORM_LINUX"
            }
            links {
                "Engine",
                "SDL2",
                "glad",
                "box2d"
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