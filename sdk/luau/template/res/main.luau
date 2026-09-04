local log = require("dusklight.log")
local host = require("dusklight.host")

log.info("My Script Mod initialized")

host.on_shutdown(function()
    log.info("My Script Mod shutting down")
end)
