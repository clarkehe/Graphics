

QtSDK =
{
	DefaultQtDir = "C:/Qt/Qt5.6.2",

	Information = {},

	PlatformUICTable =
	{
		["Windows"] = { "/bin/uic.exe" },
		["Linux"  ] = { "/bin/uic-qt5", "/bin/uic" },
		["OSX"    ] = { "/bin/uic" }
	},

	PlatformMOCTable =
	{
		["Windows"] = { "/bin/moc.exe" },
		["Linux"  ] = { "/bin/moc-qt5", "/bin/moc" },
		["OSX"    ] = { "/bin/moc" }
	},

	PlatformConfigFileTable =
	{
		["Windows"] = { "/include/QtCore/qconfig.h" },
		["Linux"  ] = { "/include/qt5/QtCore/qconfig.h", "/include/QtCore/qconfig.h" },
		["OSX"    ] = { "/lib/QtCore.framework/Headers/qconfig.h" }
	},

	PlatformIncludeTable =
	{
		["Windows"] = { "/include" },
		["Linux"  ] = { "/include/qt5", nil },
		["OSX"    ] = { nil }
	},

	ToolsetFolderTable =
	{
		["VS2015"] = "/msvc2015",
		["VS2017"] = "/msvc2017",
		["Xcode3"] = "/clang",
		["Xcode4"] = "/clang",
		["GMake" ] = "/gcc"
	},

	ArchitectureFolderTable =
	{
		["x86"] = "",
		["x64"] = "_64",
	},

	LibraryList =
	{
		"Core",
		"Gui",
		"OpenGL",
		"Widgets"
	}
}

-- Returns the first occurrence of the specified executable files in the root directory.
function QtSDK.FindExecutable( rootDir, exeFiles )
	for _, exeFile in ipairs( exeFiles ) do
		local exePath = rootDir .. exeFile
		if os.isfile( exePath ) then
			return exePath
		end
	end
	return nil
end

-- Parses the specified config file and returns the version information.
function QtSDK.GetVersion( configPath )
	local srcFile = io.open( configPath, "rb" )
	local src = srcFile:read( "*a" )
	srcFile:close()

	local _, _, major = src:find( "#define%s+QT_VERSION_MAJOR%s+(%d+)" )
	local _, _, minor = src:find( "#define%s+QT_VERSION_MINOR%s+(%d+)" )
	local _, _, patch = src:find( "#define%s+QT_VERSION_PATCH%s+(%d+)" )

	local result = {}
	result.Major = tonumber( major )
	result.Minor = tonumber( minor )
	result.Patch = tonumber( patch )
	return result
end

-- Retrieves the value of the specified macro in the specified source file.
function QtSDK.GetMacroValue( srcPath, macroName )
	local srcFile = io.open( srcPath, "rb" )
	local src = srcFile:read( "*a" )
	srcFile:close()

	local _, _, result = src:find( "#define%s+" .. macroName .. "%s+([^\n]+)" )
	return result
end

-- Returns a library table with the specified prefix and postfix.
function QtSDK.GetLibraryTable( prefix, postfix )
	local result = {}
	for _, libName in ipairs( QtSDK.LibraryList ) do
		result[libName] = prefix .. libName .. postfix
	end
	return result
end

-- Searches for Qt SDK of the specified architecture, and collects informations.
function QtSDK.GetInformation( architectureName )
	io.write( "Checking Qt5 " .. architectureName .. "... " )

	local result = {}
	local rootDirs = {}

	if GetTargetPlatformName() == "Linux" then
		table.insert( rootDirs, "/usr" )

	else
		local qtDirs = {}

		if GetTargetPlatformName() == "Windows" then
			table.insert ( qtDirs, QtSDK.DefaultQtDir)
			local logicalDiskList = io.popen( "wmic logicaldisk get name" )
			for logicalDisk in logicalDiskList:lines() do
				logicalDisk = TrimString( logicalDisk )
				if logicalDisk ~= "" and logicalDisk ~= "Name" then
					table.insert( qtDirs, logicalDisk .. "/Qt" )
				end
			end

		elseif GetTargetPlatformName() == "OSX" then
			----qt5-dir=/Users/clarke/Qt5.6.2
			local QTDIR = _OPTIONS['qt5-dir'] or ''
			if string.sub(QTDIR, -1) == '/' then
				QTDIR = string.sub(QTDIR, 1, string.len(QTDIR) - 1)
			end

			qtDirs = os.matchdirs( QTDIR )
			if next( qtDirs ) == nil then
				qtDirs = os.matchdirs( os.getenv("HOME") )
			end

		else
			print( "Not found" )
			return
		end

		for _, qtDir in ipairs( qtDirs ) do
			local sdkDirs = os.matchdirs( qtDir .. "/5.*" )
			for _, sdkDir in ipairs( sdkDirs ) do
				local rootDir = sdkDir .. QtSDK.ToolsetFolderTable[GetToolsetName()] .. QtSDK.ArchitectureFolderTable[architectureName]
				if os.isdir( rootDir ) then
					table.insert( rootDirs, rootDir )
				end
			end
		end
	end

	for _, rootDir in ipairs( rootDirs ) do
		local configFiles = QtSDK.PlatformConfigFileTable[GetHostPlatformName()]
		for configIndex = 1, #configFiles do
			local configPath = rootDir .. configFiles[configIndex]
			if os.isfile( configPath ) then
				result.Version = QtSDK.GetVersion( configPath )

				if QtSDK.GetMacroValue( configPath, "QT_REDUCE_RELOCATIONS" ) == "true" then
					result.Flags = { "PIC" }
				end

				local includeDir = QtSDK.PlatformIncludeTable[GetHostPlatformName()][configIndex]
				if includeDir ~= nil then
					result.IncludeDirectory = rootDir .. includeDir
				end

				result.UIC = QtSDK.FindExecutable( rootDir, QtSDK.PlatformUICTable[GetHostPlatformName()] )
				result.MOC = QtSDK.FindExecutable( rootDir, QtSDK.PlatformMOCTable[GetHostPlatformName()] )

				result.rootDir = rootDir
				
				local prefix, postfix
				if GetHostPlatformName() == "Windows" then
					result.LibraryDirectory = rootDir .. "/lib"
					prefix  = "Qt5"
					postfix = ""
				elseif GetHostPlatformName() == "Linux" then
					prefix  = "Qt5"
					postfix = ""
				else
					prefix  = rootDir .. "/lib/Qt"
					postfix = ".framework"
				end
				result.ReleaseLibrary = QtSDK.GetLibraryTable( prefix, postfix )

				if GetHostPlatformName() == "Windows" then
					postfix = ""
				end
				result.DebugLibrary = QtSDK.GetLibraryTable( prefix, postfix )
				break
			end
		end
	end

	if result.Version ~= nil then
		print( result.Version.Major .. "." .. result.Version.Minor .. "." .. result.Version.Patch )
		return result
	else
		print( "Not found" )
		return nil
	end
end

-- Searches for Qt SDK, and collect informations.
function QtSDK.Check()
	QtSDK.Information["x86"] = QtSDK.GetInformation( "x86" )
	QtSDK.Information["x64"] = QtSDK.GetInformation( "x64" )
end

-- Compiles the specified UI files.
function QtSDK.CompileUI( filePattern, outputDir )
	local uicPath
	if QtSDK.Information["x64"] ~= nil then
		uicPath = QtSDK.Information["x64"].UIC
	elseif QtSDK.Information["x86"] ~= nil then
		uicPath = QtSDK.Information["x86"].UIC
	end

	if uicPath == nil then
		print( "UIC not found." )
		return nil
	end

	local uiFiles = os.matchfiles( filePattern )
	local outputFiles = {}

	for _, uiFile in ipairs( uiFiles ) do
		local uiName = path.getbasename( uiFile )
		print( "Compiling Qt UI " .. uiName .. "..." )

		local outputFile = outputDir .. "/ui_" .. uiName .. ".h"
		os.execute( uicPath .. " -o " .. outputFile .. " " .. uiFile )
		table.insert( outputFiles, outputFile )
	end
	return outputFiles
end

-- Compiles the specified meta object files.
function QtSDK.CompileMetaObject( filePattern, outputDir, options )
	local mocPath
	if QtSDK.Information["x64"] ~= nil then
		mocPath = QtSDK.Information["x64"].MOC
	elseif QtSDK.Information["x86"] ~= nil then
		mocPath = QtSDK.Information["x86"].MOC
	end

	if mocPath == nil then
		print( "MOC not found." )
		return nil
	end

	local moFiles = os.matchfiles( filePattern )
	local outputFiles = {}

	for _, moFile in ipairs( moFiles ) do
		local srcFile = io.open( moFile, "rb" )
		local src = srcFile:read( "*a" )
		srcFile:close()

		if src:find( "Q_OBJECT" ) ~= nil then
			local moName = path.getbasename( moFile )
			print( "Compiling Qt MetaObject " .. moName .. "..." )

			local outputFile = outputDir .. "/moc_" .. moName .. ".h"
			local cmd = mocPath .. " -o " .. outputFile .. " -i " .. moFile
			if options ~= nil then
				cmd = cmd .. " " .. options
			end

			os.execute( cmd )
			table.insert( outputFiles, outputFile )
		end
	end
	return outputFiles
end
