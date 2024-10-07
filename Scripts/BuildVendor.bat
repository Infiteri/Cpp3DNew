@echo off

mkdir Bin

xcopy /s /y "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\"
xcopy /s /y "Engine\Vendor\YAML\yaml.dll" "Bin\"
xcopy /s /y "Engine/Vendor/glad/lib/glad.dll" "Bin\"
xcopy /s /y "Engine/Vendor/ImGui/ImGui.dll" "Bin\"
