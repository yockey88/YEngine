project "yphysx"
    location "YPhysx"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    targetdir(tdir)
    objdir(odir)

    files {
        "include/**.h",
        "src/**.c"
    }

    externalincludedirs {
        "include"
    }

    filter { "system:windows" }
        systemversion "latest"

    filter { "system:macosx" }
        systemversion "latest"
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"