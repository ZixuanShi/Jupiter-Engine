cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args="Benchmarks" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/Benchmarks/"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" vs2022 %args%

pause
