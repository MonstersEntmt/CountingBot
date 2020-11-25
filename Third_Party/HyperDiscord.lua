APP.PushGlobals()
GLOBALS.currentPath = GLOBALS.currentPath .. "vendor/json/"
local json = APP.GetOrCreateApp("JSON")
APP.PopGlobals()
json.kind = "StaticLib"
json.location = ""
json.includeDir = ""
json.sourceDir = ""
json.addLink = false

local hyperDiscord = APP.GetOrCreateApp("HyperDiscord")
hyperDiscord.kind = "StaticLib"
hyperDiscord.location = ""
hyperDiscord.includeDir = "src/HyperDiscord"
hyperDiscord.sourceDir = "src/"

hyperDiscord.AddDependency(json)

return { hyperDiscord, json }