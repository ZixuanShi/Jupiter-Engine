cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args="UnitTests" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/UnitTests/"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" vs2022 %args%

pause