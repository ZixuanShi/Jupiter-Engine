cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args=vs2022 "Benchmarks" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/Benchmarks/"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" %args%

pause
