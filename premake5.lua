workspace "easyx_framework"
    architecture "x64"

configurations { 
	"Debug", 
	"Release",
	"Dist"
}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir={}
IncludeDir["GLFW"] = "easyx_framework/plugin/GLFW/include"
IncludeDir["Glad"] = "easyx_framework/plugin/Glad/include"

include "easyx_framework/plugin/GLFW"
include "easyx_framework/plugin/Glad"

project "easyx_framework"
   location "easyx_framework"

   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir ("bin/"  .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/"  .. outputdir .. "/%{prj.name}")

   includedirs {
       "%{prj.name};",
       "%{prj.name}/Src;",
       "%{prj.name}/Src/Project;",
       "%{prj.name}/Src/Engine;",
       "%{prj.name}/Src/Engine/Component;",
       "%{prj.name}/Src/Engine/Core;",
       "%{prj.name}/Src/Engine/Object;",
       "%{prj.name}/Src/Engine/Tools;",
       "%{prj.name}/Src/Engine/Ui;",
       "%{prj.name}/Src/Project/Base;",
       "%{prj.name}/Src/Project/Level;",
       "%{prj.name}/Src/Project/Object;",
       "%{prj.name}/Src/Project/Particle;",
       "%{prj.name}/Src/Project/Sun;",
       "%{prj.name}/Src/Project/Ui;",
       "%{prj.name}/plugin/spdlog/include;",
       "%{IncludeDir.GLFW}",
       "%{IncludeDir.Glad}"
   }

   files { 
       "%{prj.name}/Src/**.h",
       "%{prj.name}/Src/**.cpp",
       "%{prj.name}/**.cfg",
       "%{prj.name}/**.txt"  
   }
   pchheader "ZHNpch.h"
   pchsource "%{prj.name}/Src/ZHNpch.cpp"

   links {
       "GLFW",
       "Glad",
       "opengl32.lib"
   }

   filter "system:windows"
      staticruntime "off"
      systemversion "latest"
      characterset "MBCS"

   filter "configurations:Debug"
      defines { "ZHN_DEBUG"
      ,"_DEBUG"}
      symbols "On"

   filter "configurations:Release"
      defines { "ZHN_RELEASE" }
      optimize "On"

   filter "configurations:Dist"
      defines { "ZHN_DIST" }
      optimize "On"
