-- MCX example script: tiny city-style RP setup
-- This script assumes that a scene named "city_rp" exists on the server side.

MCX.on("player_join", function(player)
  MCX.sendMessage(player, "Welcome to the city, " .. player.name .. "!")
  MCX.sendMessage(player, "Type !job to see available roles.")
end)

MCX.on("chat", function(player, message)
  if message == "!job" then
    MCX.sendMessage(player, "Available jobs: citizen, police")
  elseif message == "!job police" then
    MCX.setRole(player, "police")
    MCX.broadcast("[MCX] " .. player.name .. " is now part of the police.")
  elseif message == "!job citizen" then
    MCX.setRole(player, "citizen")
    MCX.broadcast("[MCX] " .. player.name .. " is now a citizen.")
  end
end)
