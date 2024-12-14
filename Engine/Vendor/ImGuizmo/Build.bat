@ECHO OFF

echo Is this a "global" or "local" build? (Type G for global, L for local)
set /p buildType="Enter G or L: "

if /i "%buildType%"=="G" (
    echo Setting up global build...
    set "cppFiles=Engine/Vendor/ImGuizmo/GraphEditor.cpp Engine/Vendor/ImGuizmo/ImCurveEdit.cpp Engine/Vendor/ImGuizmo/ImGradient.cpp Engine/Vendor/ImGuizmo/ImGuizmo.cpp Engine/Vendor/ImGuizmo/ImSequencer.cpp"
    set "INCLUDE_PATH=Engine/Vendor/ImGuizmo;Engine/Vendor/ImGui"
    set "LIBRARY_PATH=Bin"
    set "OUTPUT_FILE=Engine/Vendor/ImGuizmo/ImGuizmo.dll"
    set "LINK_LIBS=-lImGui"
) else if /i "%buildType%"=="L" (
    echo Setting up local build...
    set "cppFiles=GraphEditor.cpp ImCurveEdit.cpp ImGradient.cpp ImGuizmo.cpp ImSequencer.cpp"
    set "INCLUDE_PATH=.;../ImGui"
    set "LIBRARY_PATH=../../../Bin"
    set "OUTPUT_FILE=ImGuizmo.dll"
    set "LINK_LIBS=-lImGui"
) else (
    echo Invalid option.
    exit /b
)

:: Compile command
echo Compiling DLL...
g++ -g -shared -I%INCLUDE_PATH% -L%LIBRARY_PATH% %cppFiles% %LINK_LIBS% -o %OUTPUT_FILE%

if %ERRORLEVEL%==0 (
    echo Build succeeded! DLL saved to %OUTPUT_FILE%.
) else (
    echo Build failed. Check the error messages above.
)