local currentPath, verbose = ...

local globalApp = require("../premake/app")

local json = globalApp.app("JSON", currentPath .. "vendor/json/", verbose)
json.kind = "StaticLib"
json.location = ""
json.includeDir = ""
json.sourceDir = ""
json.addLink = false

local app = globalApp.app("HyperDiscord", currentPath, verbose)
app.kind = "StaticLib"
app.includeDir = "src/HyperDiscord"
app.sourceDir = "src/"

globalApp.addDependency(app, json, verbose)

return app