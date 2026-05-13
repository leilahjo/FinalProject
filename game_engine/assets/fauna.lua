local SPEED = 0.65
local ARENA_W = 3.7
local ARENA_H = 2.7
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
            if entity.x <= -ARENA_W + 0.1 and entity.vx < 0 then entity.vx = 0 end
            if entity.x >=  ARENA_W - 0.1 and entity.vx > 0 then entity.vx = 0 end
            if entity.y <= -ARENA_H + 0.1 and entity.vy < 0 then entity.vy = 0 end
            if entity.y >=  ARENA_H - 0.1 and entity.vy > 0 then entity.vy = 0 end
        end
    end
end
