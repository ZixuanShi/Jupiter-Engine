# Jupiter-Engine
I design, plan, write, test everything from scratch by myself

- C++ 20 Features
  - Modules. Replaced Pre-Compiled Headers for faster compilation speed
  - Concepts. Safer, and more flexible Templates magic
- Implemented my own STL
  - Containers, including DynamicArray, HashMap, SortedMap, Graph, Heap
  - Strings Library
  - Types, reflective Enums, Any, Variant, Tuple
- Implemented Json and CSV for Data-Driven
- Vulkan and DX12 renderer
- GLFW and WIN32 frameworks

# Install and Run Jupiter on Windows
- Have Visual Studio 2022, and Vulkan 1.3.296.0 installed on your machine
- Clone this despository to any location on your PC.
- Add an Environment Variable. Set the name to JUPITER_ENGINE_DIR, and the value to the path to Jupiter Engine's Root. Note it should ends with '\'. Example: C:\Program Files\Jupiter Technologies\Jupiter-Engine\
- Compile shaders. Navigate to Jupiter-Engine/Scripts, run CompileShaders - GLSL.bat
- Build Visual Studio Solution and Projects. Navigate to Jupiter-Engine/Projects/Blank/Scripts, run GenerateProjectFiles_vs2022.bat. You will have Blank/_ProjectFiles, it contains the VS solution file
