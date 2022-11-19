workspace "MachineY"
    startproject "Engine"
    architecture "x64"

    configurations {
        "Debug" ,
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

project "Engine"
    location "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(tdir)
    objdir(odir)

    files {
        "%{prj.name}/src/*.cpp"
    }

    flags { "FatalWarnings" }
