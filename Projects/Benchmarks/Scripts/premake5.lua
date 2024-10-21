function FindJupiterRootDir()
    local envVar = os.getenv("JUPITER_ENGINE_DIR")
    if envVar then
        print("Jupiter Engine directory found at: " .. envVar)
        return envVar
    else
        print("Jupiter Engine directory not found. Using default path C:/Program Files/Jupiter Technologies/Jupiter-Engine/")
        return "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"
    end
end

include (FindJupiterRootDir() .. "Scripts/premake5.lua")

context.project_name = "Benchmarks"
context.project_dir  = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/Benchmarks/"

-- Removes Debug and Release configs
context.configurations = { "Debug", "Development" }

GenerateProjectFiles()