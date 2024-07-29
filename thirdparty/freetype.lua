project "freetype"
    kind "StaticLib"

    includedirs { "freetype/include" }

    -- TODO: Rather than change these files to be searched recursively
    -- I should manually add them to ensure that there are no duplicate source files.

    files {

    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"
