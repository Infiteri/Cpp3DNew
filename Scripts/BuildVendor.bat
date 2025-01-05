@echo off

mkdir Bin

robocopy "Engine/Vendor/GLFW/lib" "Bin" glfw3.dll
robocopy "Engine/Vendor/YAML" "Bin" yaml.dll
robocopy "Engine/Vendor/glad/lib" "Bin" glad.dll
robocopy "Engine/Vendor/ImGui" "Bin" ImGui.dll
robocopy "Engine/Vendor/ImGuizmo" "Bin" ImGuizmo.dll
robocopy "Engine/Vendor/bullet/bullet3-master" "Bin" Bullet.dll
