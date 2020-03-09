function update()
    local id = get("/player")
    if id ~= nil then
        local ent1 = get("/entity", id)
        ent1.Position.x = ent1.Position.x + 0.1
        post("/entity", { entity = id, components = ent1 })
    end
end