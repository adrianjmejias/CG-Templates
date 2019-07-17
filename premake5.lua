-- premake5.lua
-- https://git-scm.com/book/en/v2/Git-Tools-Submodules
workspace "PressF"
   architecture "x64"
   startproject "PressF"
   configurations { "Debug", "Release", "Dist"}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glm"] = "dependencies/glm"
IncludeDir["spdlog"] = "dependencies/spdlog/include"
IncludeDir["Glad"] = "dependencies/Glad/include"
IncludeDir["nuklear"] = "dependencies/nuklear/"
IncludeDir["SDL"] = "dependencies/SDL/include"


group "Dependencies"
	include "dependencies/Glad"
	include "dependencies/nuklear"
group ""

project "PressF"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17" --StaticLib

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
   
   files { 
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      "dependencies/glm/glm/**.hpp",
      "dependencies/glm/glm/**.inl",
      -- "dependencies/nuklear/src/**.h",
      -- "dependencies/nuklear/src/**.c"
   }

   includedirs {
      -- internal 
      "%{prj.name}/src",

      -- dependencies
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.nuklear}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL}"
   }

   libdirs
   {
      "SDL2" ,"dependencies/SDL/lib/**"
   }

   links {
      "Glad",
      "SDL2",
      "nuklear"
   }

   -- linkoptions { "``" }
   -- prebuildcommands { "MyTool --dosomething" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"


      
project "Hazel"
   location "Hazel"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   pchheader "hzpch.h"
   pchsource "Hazel/src/hzpch.cpp"

   files
   {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      "dependencies/glm/glm/**.hpp",
      "dependencies/glm/glm/**.inl",
   }

   defines
   {
      "_CRT_SECURE_NO_WARNINGS"
   }

   includedirs
   {
     -- internal 
     "%{prj.name}/src",

     -- dependencies
     "%{IncludeDir.spdlog}",
     "%{IncludeDir.nuklear}",
     "%{IncludeDir.glm}",
     "%{IncludeDir.Glad}",
     "%{IncludeDir.SDL}"
   }

   links 
   { 
      "GLFW",
      "Glad",
      "ImGui",
      "opengl32.lib"
   }

   filter "system:windows"
      systemversion "latest"

      defines
      {
         "HZ_PLATFORM_WINDOWS",
         "HZ_BUILD_DLL",
         "GLFW_INCLUDE_NONE"
      }

   filter "configurations:Debug"
      defines {"Debug"}
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      defines {"NDebug"}
      runtime "Release"
      optimize "on"

   filter "configurations:Dist"
      defines {"DIST"}
      runtime "Dist"
      optimize "on"