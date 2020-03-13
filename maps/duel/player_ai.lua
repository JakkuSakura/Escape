function update()
    local self = get("/entity", id);
    local agents = get("/agents", { player = 0 });
    if not agents then
        return
    end

    control({
        type = "change_weapon",
        weapon = "HANDGUN"
    });

    local enemy = get("/entity", agents[1]);

    local angle = math.atan2(enemy.Position.y - self.Position.y, enemy.Position.x - self.Position.x);

    control({
        type = "shooting",
        angle = angle
    });

    if math.pow(enemy.Position.x - self.Position.x, 2) + math.pow(enemy.Position.y - self.Position.y, 2) < 15 * 15 then
        control({
            type = "move",
            x = math.cos(angle + 3.14 / 2) * 6 + math.cos(angle + 3.14) * 6,
            y = math.sin(angle + 3.14 / 2) * 6 + math.sin(angle + 3.14) * 6,
        });
    else
        control({
            type = "move",
            x = math.cos(angle + 3.14 / 2) * 6,
            y = math.sin(angle + 3.14 / 2) * 6,
        });
    end
end