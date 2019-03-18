--==================================================================================================
-- File      : premake4-common.lua
-- Brief     : Common premake script for Ling Engine.
-- Create    : 2014-10-08
-- Modify    : 2014-10-08
-- Author    : Lendy Zhang <lendyzhang@gmail.com>
-- Copyright : (C) 2008-2014 EMAX Studio, all rights reserved.
-- Website   : http://ling.emsky.net
--==================================================================================================

newoption
{
	trigger     = "target",
	value       = "PLATFORM",
	description = "Choose the target platform to build for",
	allowed     =
	{
		{ "windows", "Windows" },
		{ "linux",   "Linux" },
		{ "osx",     "OSX" },
		{ "ios",     "iOS" },
		{ "android", "Android" }
	}
}

-- Maps the toolset identifiers to name strings.
ToolsetNameTable =
{
	[ "vs2008"     ] = "VS2008",
	[ "vs2010"     ] = "VS2010",
	[ "vs2012"     ] = "VS2012",
	[ "vs2013"     ] = "VS2013",
	[ "vs2015"     ] = "VS2015",
	[ "gmake"      ] = "GMake",
	[ "xcode3"     ] = "Xcode3",
	[ "xcode4"     ] = "Xcode4",
	[ "codeblocks" ] = "CodeBlocks"
}

-- Maps the platform identifiers to name strings.
PlatformNameTable =
{
	[ "windows" ] = "Windows",
	[ "linux"   ] = "Linux",
	[ "macosx"  ] = "OSX",
	[ "osx"     ] = "OSX",
	[ "ios"     ] = "iOS",
	[ "android" ] = "Android"
}

-- Maps the architecture identifiers to name strings.
ArchitectureNameTable =
{
	[ "Native"    ] = "Native",
	[ "x32"       ] = "x86",
	[ "x64"       ] = "x64",
	[ "Universal" ] = "Universal"
}

-- Maps the platform names to dynamic library file extensions.
PlatformDynamicLibraryFileExtensionTable =
{
	[ "Windows" ] = "dll",
	[ "Linux"   ] = "so",
	[ "OSX"     ] = "dylib",
	[ "iOS"     ] = "dylib",
	[ "Android" ] = "so"
}

-- Config using LinkTimeOptimize or not
-- Note: LTO no working on NDK r12+( Confirmed on Windows Platform )
UseLinkTimeOptimize = false

----------------------------------------------------------------------------------------------------

-- Returns the name of the toolset to generate project files for.
function GetToolsetName()
	return ToolsetNameTable[ _ACTION ]
end

-- Returns the name of the host platform to build on.
function GetHostPlatformName()
	return PlatformNameTable[ os.get() ]
end

-- Returns the name of the target platform to build for.
function GetTargetPlatformName()
	if _OPTIONS["target"] ~= nil then
		return PlatformNameTable[ _OPTIONS["target"] ]
	else
		return GetHostPlatformName()
	end
end

-- Returns the output directory name.
function GetOutputDirectoryName()
	return GetToolsetName() .. "-" .. GetTargetPlatformName()
end

-- Returns the dynamic library file extension of the specified platform.
function GetDynamicLibraryFileExtension( platform )
	return PlatformDynamicLibraryFileExtensionTable[ platform ]
end

----------------------------------------------------------------------------------------------------

-- Removes leading and trailing white spaces out of a string.
function TrimString( s )
	return s:match( "^%s*(.-)%s*$" )
end

-- Returns true if the toolset is Visual Studio.
function IsVisualStudio()
	return string.match( GetToolsetName(), "VS%d%d%d%d" ) ~= nil
end

-- Returns true if the toolset is Xcode.
function IsXcode()
	return string.match( GetToolsetName(), "Xcode%d" ) ~= nil
end

-- Returns true if the target platform is Windows.
function IsTargetWindows()
	return GetTargetPlatformName() == "Windows"
end

-- Returns true if the target platform is Linux.
function IsTargetLinux()
	return GetTargetPlatformName() == "Linux"
end

-- Returns true if the target platform is OSX.
function IsTargetOSX()
	return GetTargetPlatformName() == "OSX"
end

-- Returns true if the target platform is iOS.
function IsTargetiOS()
	return GetTargetPlatformName() == "iOS"
end

-- Returns true if the target platform is OSX or iOS.
function IsTargetApple()
	return IsTargetOSX() or IsTargetiOS()
end

-- Returns true if the target platform is Android.
function IsTargetAndroid()
	return GetTargetPlatformName() == "Android"
end

-- Returns if using the configuration of LinkTimeOptimize 
function IsUseLinkTimeOptimize()
	return UseLinkTimeOptimize
end