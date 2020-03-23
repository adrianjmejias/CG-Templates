-- premake5.lua
-- https://git-scm.com/book/en/v2/Git-Tools-Submodules
workspace "PressF"
   architecture "x64"
   startproject "PressFEditor"
   configurations { "Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glm"] = "dependencies/glm"
IncludeDir["spdlog"] = "dependencies/spdlog/include"
IncludeDir["glad"] = "dependencies/glad/include"
IncludeDir["SDL"] = "dependencies/SDL/include"
IncludeDir["stb"] = "dependencies/stb"
IncludeDir["obj"] = "dependencies/OBJ-Loader/Source"
IncludeDir["ImGui"] = "dependencies/imgui/"
IncludeDir["json"] = "dependencies/json/include"
IncludeDir["filesystem"] = "dependencies/filesystem/include"
IncludeDir["assimp"] = "dependencies/assimp/include"

group "Dependencies"
	include "dependencies/glad"
   include "dependencies/imgui"
   -- include "dependencies/assimp"
   -- include "dependencies/OBJ-Loader"


   externalproject "assimp"
      location "dependencies/assimp"
      uuid "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
      kind "StaticLib"
      language "C++"
group ""

project "PressF"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17" 
   location("%{prj.name}")
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


   pchheader "%{prj.name}/pch.h"
   pchsource "PressF/PressF/pch.cpp"
   -- forceincludes { "PressF" }
   includedirs { "%{prj.name}/" } 
   files { 
      "%{prj.name}/**.h",
      "%{prj.name}/**.cpp",
      "PressF/PressF/pch.cpp",
   }

   includedirs {
      -- internal 

      -- dependencies
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.SDL}",
		"%{IncludeDir.obj}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.json}",
		"%{IncludeDir.filesystem}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.assimp}",
   }

   libdirs
   {
      "SDL2" ,
      "dependencies/SDL/lib/**"
   }

   links {
      "glad",
      "SDL2",
      "ImGui",
      -- "assimp"
      -- "ObjLoader"
   }

   filter "configurations:Debug"
      defines { "_CRT_SECURE_NO_WARNINGS", "DEBUG"}
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

      
project "PressFEditor"
kind "ConsoleApp"
language "C++"
cppdialect "C++17" --StaticLib
location("%{prj.name}")
targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files { 
   "%{prj.name}/**.h",
   "%{prj.name}/**.cpp",
}

includedirs {
   "%{IncludeDir.glm}",
   "%{IncludeDir.glad}",
   "%{IncludeDir.obj}",
   "%{IncludeDir.SDL}",
   "%{IncludeDir.spdlog}",
   "%{IncludeDir.stb}",
   "%{IncludeDir.json}",
   "%{IncludeDir.filesystem}",
   "%{IncludeDir.ImGui}",
   "%{IncludeDir.assimp}",
   "PressF"
}

libdirs
{
   "SDL2" ,
   "dependencies/SDL/lib/**",
   "PressF"
}

links {
   "glad",
   "SDL2",
   "ImGui",
   "PressF"
}


postbuildcommands {
   -- Copy the SDL2 dll to the Bin folder.
   '{COPY} "%{path.getabsolute("dependencies/SDL/lib/" .. cfg.architecture:gsub("x86_64", "x64") .. "/SDL2.dll")}" "%{cfg.targetdir}"'
}
filter "configurations:Debug"
   defines { "_CRT_SECURE_NO_WARNINGS", "DEBUG"}
   symbols "On"

filter "configurations:Release"
   defines { "NDEBUG" }
   optimize "On"