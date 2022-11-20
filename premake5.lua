workspace "MachineY"
    startproject "Engine"
    architecture "x64"

    configurations {
        "Debug" ,
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- External Dependencies
externals = {}
externals["sdl2"] = "C:\\Yock/Extern/SDL2-2.24.2"
externals["imgui"] = "C:\\Yock/Extern/imgui-1.89"
externals["entt"] = "C:\\Yock/Extern/entt-3.11.0"
externals["spdlog"] = "C:\\Yock/Extern/spdlog-1.11.0"

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
        "%{prj.name}/include/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{externals.imgui}/*.h",
        "%{externals.imgui}/*.cpp",
        "%{externals.imgui}/misc/cpp/*.h",
        "%{externals.imgui}/misc/cpp/*.cpp"
    }

    externalincludedirs {
        "Engine/include",
        "Engine/data",
        "%{externals.sdl2}/include",
        "%{externals.imgui}",
        "%{externals.imgui}/backends",
        "%{externals.imgui}/misc/cpp",
        "%{externals.entt}/single_include/entt",
        "%{externals.spdlog}/include/spdlog"
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
            "SDL2"
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
        "%{externals.imgui}/misc/cpp/*.h",
        "%{externals.imgui}/misc/cpp/*.cpp"
    }

    externalincludedirs {
        "%{prj.name}/include",
        "%{externals.sdl2}/include",
        "%{externals.imgui}",
        "%{externals.imgui}/backends",
        "%{externals.imgui}/misc/cpp",
        "%{externals.entt}/single_include/entt",
        "%{externals.spdlog}/include/spdlog"
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
            "SDL2"
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
