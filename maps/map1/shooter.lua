function update()
    local self = get("/entity", id);
    control({
        type = "change_weapon",
        weapon = "SHOTGUN"
    });

    local pos = self.Position;
    local player = get("/entity", get("/player"));

    if not player then
        return
    end
    local p = player.Position;

    local angle = math.atan2(p.y - pos.y, p.x - pos.x);

    if math.pow(p.x - pos.x, 2) + math.pow(p.y - pos.y, 2) < 30 * 30 then
        control({
            type = "shooting",
            angle = angle
        });
    end
end