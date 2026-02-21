-- MCX example script: simple lobby behavior
-- This is a draft based on docs/lua-api-design.md. The concrete implementation
-- of the MCX Lua runtime will come later.

-- When a player joins, send a welcome message and teleport them to a lobby spawn.
MCX.on("player_join", function(player)
  MCX.sendMessage(player, "Welcome to the MCX lobby, " .. player.name .. "!")
  MCX.teleport(player, { x = 0, y = 64, z = 0 })
end)

-- Simple chat trigger to switch to a test scene.
MCX.on("chat", function(player, message)
  if message == "!test" then
    MCX.loadScene("test_lobby")
    MCX.broadcast("[MCX] Switched to test_lobby scene (requested by " .. player.name .. ").")
  end
end)
