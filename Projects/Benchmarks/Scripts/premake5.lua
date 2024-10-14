function FindJupiterRootDir()
    local path = debug.getinfo(1).source
    local index = path:lower():find("jupiter%-engine")
    path = path:sub(2, index + #"Jupiter-Engine")   -- Default path C:/Program Files/Jupiter Technologies/Jupiter-Engine/
    return path
end

include (FindJupiterRootDir() .. "Scripts/premake5.lua")

context.project_name = "Benchmarks"
context.project_dir  = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/Benchmarks/"

GenerateProjectFiles()