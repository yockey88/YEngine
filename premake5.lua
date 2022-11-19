workspace "MachineY"
    startproject "Engine"
    architecture "x64"

    configurations { "Debug" , "Release" }

project "Engine"
    location "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files {
        "%{prj.name}/src/*.cpp"
    }

    flags { "FatalWarnings" }
