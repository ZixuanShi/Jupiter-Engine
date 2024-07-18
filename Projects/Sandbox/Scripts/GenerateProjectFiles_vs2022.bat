cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args="Sandbox" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/Sandbox/"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" vs2022 %args%

pause
