tc = {}

function set_module(module)
    if not tc["module"] then
        tc["module"] = module
    end
end

function init(id)
    local co = coroutine.create(
        function(id, data, len, type_name)
            while true do
                if data == "" then
                end
                tc[type_name .. "_handler"](id, data, len)
                id, data, len, type_name = coroutine.yield()
            end
            tc[id] = nil
        end
    )
    tc[id] = co
end

function recv(id, data, len, type_name)
    if not tc[id] then
        print("tc[id] is NULL")
    end
    coroutine.resume(tc[id], id, data, len, type_name)
end

_G["tc"] = tc
