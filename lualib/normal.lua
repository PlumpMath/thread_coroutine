function data_handler(id, data, len)
    print(id, data, len)
end

_G["tc"]["normal_handler"] = data_handler
