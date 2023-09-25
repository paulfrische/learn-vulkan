workspace "Vulkan Lernen"
  configurations { "Debug", "Release" }

project "learn-vulkan"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/%{cfg.buildcfg}"

  files { "src/**.cpp", "src/**.hpp" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
