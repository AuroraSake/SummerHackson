workspace "Fruits"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release"
    }

-- solution include includeDirectories from root folder
outputDiractory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architechture}"

project "Lemon"
    location "Lemon"
    kind "WindowedApp"
    language "C++"
    
    targetdir ("bin/" .. outputDiractory .. "/%{prj.name}")
    objdir("bin-inter/" .. outputDiractory .. "/%{prj.name}")

    defines 
    {}

    syslibdirs
    {
        "$(LibraryPath)",
        "$(DXSDK_DIR)Lib/x64",
        "$(VC_LibraryPath_x64)"
    }

    externalincludedirs 
    {
        "$(DXSDK_DIR)Include",
        "$(IncludePath)",
        "$(ProjectDir)"
    }

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src/Lemon",
        "%{prj.name}/Legacy"
    }

    debugdir "$(SolutionDir)"

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"