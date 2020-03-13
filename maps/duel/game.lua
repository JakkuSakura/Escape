player_death = 0;
function update()
    post("/map_info", { key = "player_death", value = be_killed });

    local id = get("/player");
    if not id then
        player_death = player_death + 1;

        post("/agents", {
            x = 9,
            y = -9;
            player = 1,
            group = 1,
            ai = "player_ai"
        });
    end
    local agents = get("/agents");
    if #agents < 2 then
        post("/agents", {
            x = 40,
            y = -40;
            player = 0,
            group = 0,
            ai = "simple_ai"
        });
    end
end

function on_collision(a, b)
    --    print(a .. " and " .. b .. " collided");
end