local CHASE_RADIUS = 1.0
local SPEED = 0.25

-- Return the function directly to the C++ Behavior::load method.
return function(entities, entityProvider)
    -- Fetch the player's Entity using the global PLAYER_ID injected by C++
    local player = entityProvider:getEntity(PLAYER_ID)
    for i = 1, #entities do
        local entity = entities[i]
        local dx = player.x - entity.x
        local dy = player.y - entity.y
        local distSq = (dx * dx) + (dy * dy)
        if distSq < (CHASE_RADIUS * CHASE_RADIUS) then
            local dist = math.sqrt(distSq)
            entity.vx = - (dx / dist) * SPEED
            entity.vy = - (dy / dist) * SPEED
        elseif entity.vx ~= 0 or entity.vy ~= 0 then
            entity.vx = 0
            entity.vy = 0
        end
    end
end