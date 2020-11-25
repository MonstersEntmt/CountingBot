APP = require("premake/app")
UTILS = require("premake/utils")

APP.SetWorkspaceName("CountingBot")
UTILS.AddPlatforms()
UTILS.AddConfigurations()

local apps = APP.GetLocalApp()
APP.SetStartApp(apps[1])

APP.PremakeWorkspace()