local hyperDiscord = APP.GetThirdPartyLibrary("HyperDiscord")

local countingBot = APP.GetOrCreateApp("CountingBot")
countingBot.group = "Apps"
countingBot.kind = "ConsoleApp"

countingBot.AddDependency(hyperDiscord[1])

countingBot.AddState({}, function()
	links({ "Winhttp.lib", "fwpuclnt.lib", "ntdsapi.lib" })
end)
countingBot.AddState("system:linux", function()
	linkoptions({ "-pthread -Wl, -rpath=\\$$ORIGIN" })
	links({ "dl" })
end)
countingBot.AddState({ "system:macosx or ios", "files:**.cpp" }, function()
	compileas("Objective-C++")
end)

return { countingBot }