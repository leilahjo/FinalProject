local SPEED = 1.8
local printed = false

-- Return the function directly to the C++ Behavior::load method.
return function(entities, entityProvider)
    if not printed then
        print("lua running")
        printed = true
    end

    local player = entityProvider:getEntity(PLAYER_ID)
    if not player then
        return
    end

    for i = 1, #entities do
        local entity = entities[i]
        local dx = player.x - entity.x
        local dy = player.y - entity.y
        local dist = math.sqrt(dx * dx + dy * dy)
        if dist > 0.001 then
            entity.vx = (dx / dist) * SPEED
            entity.vy = (dy / dist) * SPEED
        end
    end
end
