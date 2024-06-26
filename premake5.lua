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

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "Lemon/src",
        "$(IncludePath)",
        "$(DXSDK_DIR)Include"
    }

    links
    {
        "Lemon",
        "$(DXSDK_DIR)Lib\x64",
        "$(LibraryPath)"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"