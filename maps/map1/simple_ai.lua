function update()
    local self = get("/entity", id);
    post({
        type = "change_weapon",
        weapon = "SHOTGUN"
    });

    local pos = self.Position;
    local player = get("/entity", get("/player", ""));

    if not player then
        return
    end
    local p = player.Position;

    local angle = math.atan2(p.y - pos.y, p.x - pos.x);

    if math.pow(p.x - pos.x, 2) + math.pow(p.y - pos.y, 2) < 15 * 15 then
        post({
            type = "shooting",
            angle = angle
        });
        post({
            type = "move",
            x = math.cos(angle) * 6,
            y = math.sin(angle) * 6,
        });
    end
end