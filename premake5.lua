workspace "stacked"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    startproject "stacked"

project "stacked"
    kind "SharedLib"
    language "C++"
    cppdialect "C++2a"
    staticruntime "On"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    includedirs {
        "include",
        "thirdparty/glad/include",
        "thirdparty/glfw/include",
        "thirdparty/freetype/include",
        "thirdparty/glm"
    }

    files {
        "src/**.cpp",
        "src/**.h",
    }

    links {
        "GLAD",
        "GLFW",
        "FreeType"
    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"

project "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++2a"
    staticruntime "On"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    includedirs {
        "include",
        "thirdparty/glad/include",
        "thirdparty/glfw/include",
        "thirdparty/freetype/include",
        "thirdparty/glm"
    }

    files {
        "sandbox/**.cpp",
        "sandbox/**.h",
    }

    links {
        "GLAD",
        "GLFW",
        "stacked",
    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"

include "thirdparty/freetype"
include "thirdparty/glad"
include "thirdparty/glfw"
