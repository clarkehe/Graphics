--==================================================================================================
-- File      : premake4.lua
-- Brief     : Premake script
--==================================================================================================

require "premake4-common"

newoption
{
	trigger     = "uic",
	description = "Compile Qt UI files."
}

newoption
{
	trigger     = "moc",
	description = "Compile Qt meta object files."
}

newoption
{
	trigger     = "with-cg",
	description = "Use Cg shading language."
}

newoption
{
	trigger     = "qt5-dir",
	description = "Specified the directory of Qt5."
}

newoption
{
	trigger     = "editor",
	description = "Use Ling Editor Mode."
}

-- Checks options and targets.
if not _ACTION then
	return
else
	if GetToolsetName() == nil then
		print "ERROR: Unsupported toolset!"
		os.exit()
	elseif GetTargetPlatformName() == nil then
		print "ERROR: Unsupported platform!"
		os.exit()
	end
end

-- The configuration types.
local ConfigurationTypes =
{
	"Release",
	"Debug",
	"Profile"
}

-- The source file extensions.
local SourceFileExtensions =
{
	"h", "hpp", "hxx", "inl",
	"c", "cpp", "cxx", "m", "mm",

	--for tools base on qt
	"ui", "qrc", "ts"
}

-- The source directories.
local SourceDirectories =
{
	"Include",
	"Source"
}

-- The source directory that contains precompiled header file.
local PCHSourceDirectory = "Source"

-- The source directories for each platform.
local PlatformSourceDirectoryTable =
{
	[ "Windows" ] = { "Windows" },
	[ "Linux"   ] = { "Linux", "Unix" },
	[ "OSX"     ] = { "OSX", "Apple", "Unix" },
	[ "iOS"     ] = { "iOS", "Apple", "Unix" },
	[ "Android" ] = { "Android", "Unix" }
}

----------------------------------------------------------------------------------------------------

-- Returns the configuration names with specified suffixes.
function GetConfigurationList( ... )
	local configList = {}

	for _, configType in ipairs( ConfigurationTypes ) do
		for _, configSuffix in ipairs( arg ) do
			if type( configSuffix ) == "table" then
				for _, suffix in ipairs( configSuffix ) do
					table.insert( configList, configType .. suffix )
				end
			else
				table.insert( configList, configType .. configSuffix )
			end
		end
	end

	return configList
end

-- Returns the configuration filter for specified suffixes.
function GetConfigurationFilter( ... )
	return table.concat( GetConfigurationList( arg ), " or " )
end

----------------------------------------------------------------------------------------------------

-- Adds files to the project and sets precompiled header if specified.
function AddFiles( rootPath, pchName )
	local fileList    = {}
	local excludeList = {}
	local vpathSource = {}
	local vpathCommon = {}

	for _, sourceDir in ipairs( SourceDirectories ) do
		local commonPath   = "../Code/Common/" .. sourceDir
		local sourcePath   = rootPath .. "/" .. sourceDir
		local platformRoot = sourcePath .. "/Platforms"

		-- Add all source files.
		for _, sourceExt in ipairs( SourceFileExtensions ) do
			table.insert( fileList, rootPath .. "/**." .. sourceExt )
			table.insert( fileList, commonPath .. "/**." .. sourceExt )
			table.insert( fileList, sourcePath .. "/**." .. sourceExt )
		end

		-- Exclude files that are not for the target platform.
		local platformDirs  = PlatformSourceDirectoryTable[ GetTargetPlatformName() ]
		local platformPaths = os.matchdirs( platformRoot .. "/*" )

		for _, platformPath in ipairs( platformPaths ) do
			if not table.contains( platformDirs, path.getname( platformPath ) ) then
				table.insert( excludeList, platformPath .. "/**" )
			end
		end

		-- Add virtual paths.
		for _, platformDir in ipairs( platformDirs ) do
			table.insert( vpathSource, platformRoot .. "/" .. platformDir .. "/**" )
		end

		table.insert( vpathSource, sourcePath .. "/**" )
		table.insert( vpathCommon, commonPath .. "/**" )
	end

	files    ( fileList )
	excludes ( excludeList )
	vpaths   { [ "*" ] = vpathSource, [ "Common/*" ] = vpathCommon }

	-- Set precompiled header if specified.
	if pchName ~= nil then
		pchheader ( pchName .. ".h" )
		pchsource ( rootPath .. "/" .. PCHSourceDirectory .. "/" .. pchName .. ".cpp" )
	end
end

-- Adds include directories to the project.
function AddIncludes( includeDirList )
	for _, includeDir in ipairs( includeDirList ) do
		for _, platformDir in ipairs( PlatformSourceDirectoryTable[ GetTargetPlatformName() ] ) do
			includedirs { includeDir .. "/Platforms/" .. platformDir }
		end

		includedirs { includeDir }
	end
end

----------------------------------------------------------------------------------------------------

-- Sets project kind.
function SetKind( projectKind, rootPath )
	if IsTargetiOS() and projectKind == "ConsoleApp" then
		projectKind = "WindowedApp"
	elseif IsTargetAndroid() and projectKind ~= "StaticLib" then
		projectKind = "SharedLib"
	end

	kind( projectKind )

	if IsTargetApple() and projectKind == "WindowedApp" and rootPath ~= nil then
		files { rootPath .. "/Resource/" .. GetTargetPlatformName() .. "/Info.plist" }
	end
end

-- Sets library directories.
function SetLibraryDirectories()
	local platformList = platforms()

	for _, platformName in ipairs( platformList ) do
		configuration { platformName }
		libdirs       { "../External/Lib/" .. GetOutputDirectoryName() .. "/" .. ArchitectureNameTable[ platformName ] }
	end

	configuration {}
end

-- Sets target output directories.
function SetTargetDirectories()
	local platformList = platforms()

	for _, platformName in ipairs( platformList ) do
		local binLocation = "../Bin/" .. GetOutputDirectoryName() .. "/" .. ArchitectureNameTable[ platformName ]
		local libLocation = "../Lib/" .. GetOutputDirectoryName() .. "/" .. ArchitectureNameTable[ platformName ]

		configuration { platformName, "SharedLib or ConsoleApp or WindowedApp" }
		targetdir     ( binLocation )

		configuration { platformName, "StaticLib" }
		targetdir     ( libLocation )

		if IsTargetWindows() then
			configuration { platformName, "SharedLib" }
			implibdir     ( libLocation )
		end
	end

	configuration {}

	if not IsTargetiOS() and not IsTargetAndroid() then
		if _OPTIONS["editor"] then
			defines  { "LING_EDITOR" }
		end
	end
end

----------------------------------------------------------------------------------------------------

-- Configures OpenGL.
function SetOpenGL()
	if IsTargetWindows() then
		links { "OpenGL32" }
	end

	if IsTargetLinux() then
		links { "GL" }
	end

	if IsTargetApple() then
		links { "CoreFoundation.framework", "CoreGraphics.framework" }

		if IsTargetOSX() then
			links { "OpenGL.framework", "AppKit.framework" }
		else
			links { "OpenGLES.framework", "UIKit.framework", "QuartzCore.framework" }
		end
	elseif IsTargetAndroid() then
		links { "EGL", "GLESv2" }
	else
		defines { "GLEW_STATIC" }

		configuration { "Debug*" }
			links { "glew_d" }

		configuration { "Release* or Profile*" }
			links { "glew" }

		configuration {}
	end
end

----------------------------------------------------------------------------------------------------
require 'premake4-qt'

QtSDK.Check()

-- Configures Qt.
function SetQt()
	if not (IsTargetWindows() or IsTargetLinux() or IsTargetOSX()) then return end
	local platformList = platforms()

	for _, platformName in ipairs( platformList ) do
		local archName = ArchitectureNameTable[platformName]
		if archName ~= "x86" and archName ~= "x64" then
			archName = "x64"
		end

		local info = QtSDK.Information[archName]
		if info ~= nil then
			configuration { platformName }

			if info.LibraryDirectory ~= nil then
				libdirs { info.LibraryDirectory }
			end

			if info.IncludeDirectory ~= nil then
				includedirs { info.IncludeDirectory }
			end

			if info.Flags ~= nil then
				flags ( info.Flags )
			end

			configuration { "Debug*" }
				links { info.DebugLibrary["Core"], info.DebugLibrary["Gui"], info.DebugLibrary["OpenGL"], info.DebugLibrary["Widgets"] }

			configuration { "Release* or Profile*" }
				links { info.ReleaseLibrary["Core"], info.ReleaseLibrary["Gui"], info.ReleaseLibrary["OpenGL"], info.ReleaseLibrary["Widgets"] }

			configuration {}
		end
	end
end

-- Compiles Qt UI files and MetaObject files in the specified directory.
function CompileQtFiles( rootDir, outputDir )
	os.mkdir( outputDir )

	--set output dir path
	custombuild_qtgendir ( outputDir )

	--set moc/uic bin path
	local RootDir_x32 = nil
	if hostName == 'Windows' then
		RootDir_x32 = QtSDK.Information["x86"].rootDir
	end
	local RootDir_x64 = QtSDK.Information["x64"].rootDir

	if RootDir_x32 then
		custombuild_qtbin    ( RootDir_x32.."/bin" )
	end

	if RootDir_x64 then
		custombuild_qtbin    ( RootDir_x64.."/bin" )
	end

	--create *.h file
	if _OPTIONS["uic"] then
		local uiFiles = QtSDK.CompileUI( rootDir .."/Source/FromFiles/**.ui", outputDir )
		files { uiFiles }
	end

	if _OPTIONS["moc"] then
		local mocFiles = QtSDK.CompileMetaObject( rootDir .. "/Include/**.h", outputDir, "-D Ling_BeginNamespace=\"namespace Ling {\" -D Ling_EndNamespace=\"}\"" )
		files { mocFiles }
	end

	includedirs { outputDir }
end

-- Configures Cg Toolkit.
function SetCgToolkit()
	if not _OPTIONS["with-cg"] then
		return
	end

	if IsTargetWindows() then
		links       { "Cg", "CgGL" }
		includedirs { "$(CG_INC_PATH)" }

		configuration { "x32" }
			libdirs { "$(CG_LIB_PATH)" }

		configuration { "x64" }
			libdirs { "$(CG_LIB64_PATH)" }

		configuration {}
	end

	if IsTargetLinux() then
		links { "Cg", "CgGL" }
	end

	if IsTargetOSX() then
		links { "/Library/Frameworks/Cg.framework" }
	end
end

function SetUpGlew( )
	includedirs { "../External/glew-2.1.0/include" }

	if IsTargetOSX() then
		links { "GLEW" }
		libdirs { "../External/glew-2.1.0/lib"}
	end
end

function SetUpGlfw( )
	includedirs { "../External/glfw-3.2.1/include" }

	if IsTargetOSX() then
		links { "glfw3" }
		libdirs { "../External/glfw-3.2.1/build/src/Debug"}
	end
end

function SetUpGlm( )
	includedirs { "../External/glm-0.9.9.3" }

	if IsTargetOSX() then
		links { "glm_static" }
		libdirs { "../External/glm-0.9.9.3/build/glm/Debug"}
	end

	defines { "GLM_ENABLE_EXPERIMENTAL" }
end

function SetUpAssimp( )
	includedirs { "../External/assimp-4.0.1/include" }
	includedirs { "../External/assimp-4.0.1/build/include"}

	if IsTargetOSX() then
		links { "assimp" }
		links { "IrrXML" }
		links { "zlibstatic"}

		libdirs { "../External/assimp-4.0.1/build/code/Debug"}
		libdirs { "../External/assimp-4.0.1/build/contrib/irrXML/Debug"}
		libdirs { "../External/assimp-4.0.1/build/contrib/zlib/Debug"}
	end
end

function CopyBinFiles()
	local hostName = GetHostPlatformName()
	if hostName == 'Linux' then return end
	if not (IsTargetWindows() or IsTargetLinux() or IsTargetOSX()) then return end

	local RootDir_x32 = nil
	if hostName == 'Windows' then
		RootDir_x32 = QtSDK.Information["x86"].rootDir
		print ("RootDir_x32: " .. RootDir_x32)
	end

	local RootDir_x64 = QtSDK.Information["x64"].rootDir
	print ("RootDir_x64: " .. RootDir_x64)

	QTPath = {}
	QTPath.RootDir_x32 = RootDir_x32 
	QTPath.RootDir_x64 = RootDir_x64
	
	local fbxsdkLib
	if hostName == 'OSX' then
		local fbxsdk = FBXSDK.Information
		if not fbxsdk then
			print( 'Something wrong with FBXSDK.' )
			os.exit()
		end

		fbxsdkLib = os.matchfiles( fbxsdk.RootDirectory .. '/**/clang/release/libfbxsdk.dylib')[1]
		if not fbxsdkLib then
			print( 'Please install fbx sdk first' )
			os.exit()
		end
	end

	local execute = os.execute
	for _, platformName in ipairs( platforms() ) do
		local binLocation = "../Bin/" .. GetOutputDirectoryName() .. "/" .. ArchitectureNameTable[ platformName ]
		if hostName == 'OSX' and IsTargetOSX() then
			execute( 'mkdir -p ' .. binLocation )
			if fbxsdkLib then
				fbxsdkLib = string.gsub( fbxsdkLib, ' ', '\\ ' )
				execute( 'cp ' .. fbxsdkLib .. ' ' .. binLocation )
			end
			local cmd = 'cp -r ' .. RootDir_x64 .. '/lib/' .. '%s ' .. binLocation
			execute( string.format( cmd, 'QtCore.framework' ) )
			execute( string.format( cmd, 'QtGui.framework' ) )
			execute( string.format( cmd, 'QtOpenGL.framework' ) )
			execute( string.format( cmd, 'QtWidgets.framework' ) )
		elseif hostName == 'Linux' then
			-- todo
		elseif hostName == 'Windows' then
			binLocation = string.gsub( binLocation, '/', '\\' )
			if QTPath['RootDir_' .. ArchitectureNameTable[ platformName ]] then
				local dllDir = QTPath['RootDir_' .. ArchitectureNameTable[ platformName ]] .. '\\bin'
				dllDir = string.gsub( dllDir, '/', '\\' )
				execute( 'md ' .. binLocation )
				local cmd = 'copy ' .. dllDir .. '\\%s ' .. binLocation
				execute( string.format( cmd, 'Qt5Cored.dll' ) )
				execute( string.format( cmd, 'Qt5Guid.dll' ) )
				execute( string.format( cmd, 'Qt5OpenGLd.dll' ) )
				execute( string.format( cmd, 'Qt5Widgetsd.dll' ) )
				execute( string.format( cmd, 'Qt5Core.dll' ) )
				execute( string.format( cmd, 'Qt5Gui.dll' ) )
				execute( string.format( cmd, 'Qt5OpenGL.dll' ) )
				execute( string.format( cmd, 'Qt5Widgets.dll' ) )
				execute( 'echo ResourceCompilerCLI_d.exe -a > ' .. binLocation .. '\\ResourceCompilerCLI.bat' )
				local platformDir = QTPath['RootDir_' .. ArchitectureNameTable[ platformName ]] .. '\\plugins\\platforms'
				platformDir = string.gsub( platformDir, '/', '\\' )
				execute( 'mkdir ' .. binLocation .. '\\platforms' )
				execute( 'xcopy /E /Y ' .. platformDir .. ' ' .. binLocation .. '\\platforms' )
			end
		end
	end
end

-- DisableWarning
function DisableWarning()
	if IsTargetOSX() or IsTargetiOS() then
		buildoptions {
			"-Wno-unused-variable",
			"-Wno-unused-parameter",
			"-Wno-unused-private-field",
			"-Wno-sign-compare",
			"-Wno-reorder",
			"-Wno-unreachable-code",
			"-Wno-deprecated-declarations",
			"-Wno-invalid-offsetof",
			"-Wno-unused-function",
			"-Wno-missing-field-initializers",
			"-Wno-bitwise-op-parentheses",
			"-Wno-sentinel",
			"-Wno-unused-value",
			"-Wno-switch"
		}
	end

	if IsTargetiOS() then
		buildoptions {
			"-Wno-shorten-64-to-32"
		}
	end
end

----------------------------------------------------------------------------------------------------
-- Solution

solution "Graphics"
	location   ( GetOutputDirectoryName() )
	language   "C++"
	cxxdialect "C++11"

	if IsXcode() then
		configurations ( GetConfigurationList( "" ) )

		if IsTargetOSX() then
			kind "SharedLib"
		else
			kind "StaticLib"
		end
	else
		configurations ( GetConfigurationList( "Dll", "Lib" ) )

		configuration { GetConfigurationFilter( "Dll" ) }
			kind "SharedLib"

		configuration { GetConfigurationFilter( "Lib" ) }
			kind "StaticLib"

		configuration {}
	end

	if IsTargetApple() or IsTargetAndroid() then
		platforms { "Native" }
	else
		platforms { "x32", "x64" }
	end

	flags       { "ExtraWarnings", "FatalWarnings", "RuntimeSearchPath", "Symbols" }
	--includedirs { "../External/EASTL/include", "../External/GLEW/include", "../External/freetype/include", "../External/RapidJSON/include", "../Code/Common/Include" }
	objdir      ( "../Temp/" .. GetOutputDirectoryName() )

	SetLibraryDirectories()

	if IsTargetAndroid() then
		ccompiler   "$(NDK_ROOT)/bin/clang"
		cxxcompiler "$(NDK_ROOT)/bin/clang++"

		if IsUseLinkTimeOptimize() then
			archiver  ( "$(NDK_ROOT)/arm-linux-androideabi/bin/ar --plugin $(NDK_ROOT)/lib64/LLVMgold." .. GetDynamicLibraryFileExtension( GetHostPlatformName() ) )
		else
			archiver  ( "$(NDK_ROOT)/arm-linux-androideabi/bin/ar" )
		end

		flags       { "EnableNEON" }
		links       { "android", "log" }
		includedirs { "../External/AndroidUtil/include" }
	else
		flags { "EnableSSE2", "EnableSSE4" }
	end

	if IsTargetWindows() then
		defines { "WIN32", "_WIN32" }
		links   { "Rpcrt4" }

		configuration { "x64" }
			defines { "WIN64", "_WIN64" }

		configuration { "Release*" }
			implibsuffix ".dll"

		configuration { "Debug*" }
			implibsuffix "_d.dll"

		configuration { "Profile*" }
			implibsuffix "_p.dll"

		configuration {}
			flags { "NoMinimalRebuild" }
			buildoptions { "/MP" }
	end

	if IsTargetLinux() or IsTargetAndroid() then
		buildoptions { "-Wno-type-limits", "-Wno-unused-result", "-Wno-unused-local-typedefs", "-Wno-array-bounds" }
	end

	if IsVisualStudio() then
		buildoptions { "/utf-8" }
	end

	if IsTargetOSX() then
		deploymenttarget "OSX10.8"
	end

	if IsTargetiOS() then
		deploymenttarget "iOS7.0"
	end

	if GetToolsetName() == "VS2015" then
		configuration { "Debug*" }
			defines { "_ALLOW_RTCc_IN_STL" }

		configuration {}
	end

	configuration { "Debug*" }
		defines      { "DEBUG", "_DEBUG" }
		targetsuffix "_d"
		--links        { "eastl_d" }

	configuration { "Profile*" }
		targetsuffix "_p"

	configuration { "Release* or Profile*" }

		if IsUseLinkTimeOptimize() then
			flags   { "OptimizeFull", "LinkTimeOptimize" }
		else
			flags   { "OptimizeFull" }
		end

		defines { "NDEBUG", "_NDEBUG" }
		--links   { "eastl" }

	configuration {}

	--CopyBinFiles()
	DisableWarning()

----------------------------------------------------------------------------------------------------
-- Projects

project "Math"
	uuid "EAA6F3CD-712B-4671-AEDB-7D56DDF0DBF2"
	
	if IsTargetLinux() then
		links { "rt", "uuid" }
	end

	if IsTargetOSX() then
		links { "CoreServices.framework" }
	end

	AddFiles    ( "../Math", "stdafx" )
	AddIncludes { "../Math" }

	if not IsTargetWindows() then
		excludes ("../Math/targetver.*")
	end

	SetTargetDirectories()

	configuration { "SharedLib" }
		defines { "_MATH_DLL" }

	configuration { "StaticLib" }
		defines { "_MATH_LIB" }

	configuration {}


project "RayTracer"
	uuid  "13A070B9-2D92-4512-8296-1ECEDAB34B31"
	kind  "ConsoleApp"
	links { "Math" }

	if IsTargetLinux() then
		links { "dl" }
	end

	AddFiles    ( "../RayTracer", "stdafx" )
	AddIncludes { "../RayTracer" }

	if not IsTargetWindows() then
		excludes "../RayTracer/targetver.*"
	end

	SetTargetDirectories()
	SetOpenGL()

project "SoftRasterization"
	uuid  "13A070B9-2D92-4512-8296-1ECEDAB34B32"
	kind  "ConsoleApp"
	links { "Math" }

	if IsTargetLinux() then
		links { "dl" }
	end

	AddFiles    ( "../SoftRasterization/src", "stdafx" )
	AddFiles    ( "../SoftRasterization/common", nil )
	AddFiles    ( "../SoftRasterization", nil )

	excludes  ("../SoftRasterization/external/**.*")
	excludes  ("../SoftRasterization/3rdparty/**.*")

	AddIncludes { "../SoftRasterization/src", "../SoftRasterization/common", "../SoftRasterization" }

	if not IsTargetWindows() then
		excludes "../SoftRasterization/targetver.*"
	end

	SetTargetDirectories()
	SetOpenGL()
	SetUpGlew()
	SetUpGlfw()
	SetUpGlm()
	SetUpAssimp()


