project "GLFW"
    kind "StaticLib"
    language "C"
    pic "On"

    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    includedirs { "glfw/include" }

    defines { "_CRT_SECURE_NO_WARNINGS" }

    files {
        "glfw/src/glfw_config.h",
	    "glfw/src/context.c",
	    "glfw/src/init.c",
    	"glfw/src/input.c",
	    "glfw/src/monitor.c",
	    "glfw/src/vulkan.c",
	    "glfw/src/window.c",
	    "glfw/src/platform.c",
	    "glfw/src/null_init.c",
	    "glfw/src/null_monitor.c",
	    "glfw/src/null_window.c",
	    "glfw/src/null_joystick.c"
    }

    filter { "system:Linux" }
        systemversion "latest"
        staticruntime "On"

        files {
            "glfw/src/x11_init.c",
	        "glfw/src/x11_monitor.c",
	        "glfw/src/x11_window.c",
	        "glfw/src/xkb_unicode.c",
	        "glfw/src/posix_time.c",
	        "glfw/src/posix_thread.c",
	        "glfw/src/glx_context.c",
	        "glfw/src/egl_context.c",
	        "glfw/src/osmesa_context.c",
	        "glfw/src/linux_joystick.c",
	        "glfw/src/posix_module.c",
	        "glfw/src/posix_poll.c"
        }

        defines { "_GLFW_X11" }

    filter { "system:Windows" }
        systemversion "latest"
        staticruntime "On"

        files {
            "glfw/src/win32_init.c",
	        "glfw/src/win32_joystick.c",
	        "glfw/src/win32_module.c",
	        "glfw/src/win32_monitor.c",
	        "glfw/src/win32_thread.c",
	        "glfw/src/win32_time.c",
	        "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines { "_GLFW_WIN32" }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release or configurations:Dist" }
        runtime "Release"
        optimize "On"