workspace 'Vulkan Lernen'
  configurations { 'Debug', 'Release' }

project 'learn-vulkan'
  kind 'ConsoleApp'
  language 'C++'
  cppdialect 'C++20'
  targetdir 'bin/%{cfg.buildcfg}'

  links { 'SDL2', 'vulkan', 'spdlog', 'fmt' }
  libdirs {
            os.findlib('SDL2'),
            os.findlib('vulkan'),
            os.findlib('spdlog'),
          }

  files { 'src/**.cpp', 'src/**.hpp' }

  filter 'configurations:Debug'
    defines { 'DEBUG' }
    symbols 'On'

  filter 'configurations:Release'
    defines { 'NDEBUG' }
    optimize 'On'
