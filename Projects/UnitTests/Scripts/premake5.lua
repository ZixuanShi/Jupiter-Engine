function FindJupiterRootDir()
    local envVar = os.getenv("JUPITER_ENGINE_ROOT")
    if envVar then
        print("Jupiter Engine directory found at: " .. envVar)
        return envVar
    else
        print("Jupiter Engine directory not found. Using default path C:/Program Files/Jupiter Technologies/Jupiter-Engine")
        return "C:/Program Files/Jupiter Technologies/Jupiter-Engine"
    end
end

function GetProjectDir()
    local projectDir = path.getdirectory(_SCRIPT)
    projectDir = projectDir:sub(1, projectDir:find("Scripts") - 1)
    return projectDir
end

include (FindJupiterRootDir() .. "/Scripts/Premake5.lua")

g_context.project_name = "UnitTests"
g_context.project_dir  = GetProjectDir()

GenerateProjectFiles()