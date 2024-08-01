workspace "stacked"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    startproject "stacked"

project "stacked"
    kind "StaticLib"
    language "C++"
    cppdialect "C++2a"
    staticruntime "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    includedirs {
        "include",
        "thirdparty/glad/include",
        "thirdparty/glfw/include",
        "thirdparty/freetype/include"
    }

    files {
        "src/**.cpp",
        "src/**.h",
    }

    links {
        "GLAD",
        "GLFW"
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
        "dl",
        "pthread",
        "stacked",
        "FreeType"
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
