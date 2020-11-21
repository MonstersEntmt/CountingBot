local currentPath, verbose = ...

local globalApp = require("premake/app")

local hyperDiscord = globalApp.third_party_library("HyperDiscord", currentPath, verbose)

local countingBot = globalApp.app("CountingBot", currentPath, verbose)
countingBot.group = "Apps"
countingBot.kind = "ConsoleApp"

globalApp.addDependency(countingBot, hyperDiscord, verbose)

globalApp.addState(countingBot, { filter = "system:linux", premakeState = function()
	linkoptions { "-pthread -Wl, -rpath=\\$$ORIGIN" }
	links { "dl" }
end}, verbose)
globalApp.addState(countingBot, { filter = "system:ios", premakeState = function()
	files {
		countingBot.currentPath .. countingBot.resourceDir .. "Info.plist",
		countingBot.currentPath .. countingBot.resourceDir
	}
end}, verbose)
globalApp.addState(countingBot, { filter = { "system:macosx or ios", "files:**.cpp" }, premakeState = function()
	compileas "Objective-C++"
end}, verbose)

return countingBot