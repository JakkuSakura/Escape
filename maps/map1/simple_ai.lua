--[[
print_dump是一个用于调试输出数据的函数，能够打印出nil,boolean,number,string,table类型的数据，以及table类型值的元表
参数data表示要输出的数据
参数showMetatable表示是否要输出元表
参数lastCount用于格式控制，用户请勿使用该变量
]]
function print_dump(data, showMetatable, lastCount)
    if type(data) ~= "table" then
        --Value
        if type(data) == "string" then
            io.write("\"", data, "\"")
        else
            io.write(tostring(data))
        end
    else
        --Format
        local count = lastCount or 0
        count = count + 1
        io.write("{\n")
        --Metatable
        if showMetatable then
            for i = 1, count do io.write("\t") end
            local mt = getmetatable(data)
            io.write("\"__metatable\" = ")
            print_dump(mt, showMetatable, count) -- 如果不想看到元表的元表，可-*将showMetatable处填nil
            io.write(",\n") --如果不想在元表后加逗号，可以删除这里的逗号
        end
        --Key
        for key, value in pairs(data) do
            for i = 1, count do io.write("\t") end
            if type(key) == "string" then
                io.write("\"", key, "\" = ")
            elseif type(key) == "number" then
                io.write("[", key, "] = ")
            else
                io.write(tostring(key))
            end
            print_dump(value, showMetatable, count) -- 如果不想看到子table的元表，可将showMetatable处填nil
            io.write(",\n") --如果不想在table的每一个item后加逗号，可以删除这里的逗号
        end
        --Format
        for i = 1, lastCount or 0 do io.write("\t") end
        io.write("}")
    end
    --Format
    if not lastCount then
        io.write("\n")
    end
end

function update()
    local self = get(id);

    post({
        type = "change_weapon",
        weapon = "SHOTGUN"
    });

    local pos = self.Position;
    local player = get("player");
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