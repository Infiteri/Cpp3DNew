@ECHO OFF

echo Is this a "global" or "local" build? (Type G for global, L for local)
set /p buildType="Enter G or L: "

if /i "%buildType%"=="L" (
    echo Setting up local build
    set "SRC_DIRS=code"
    set "INCLUDE_PATH=code"
    set "OUTPUT_FILE=assimp.dll"
) else if /i "%buildType%"=="G" (
    echo Setting up global build...
) else (
    echo Invalid option.
    exit /b
)

setlocal EnableDelayedExpansion
set "CPP_FILES="
for %%D in (%SRC_DIRS%) do (
    for %%F in (%%D\*.cpp) do (
        if exist %%F (
            set "CPP_FILES=!CPP_FILES! %%F"
        )
    )
)

echo !CPP_FILES!

g++ -g -shared -I%INCLUDE_PATH% !CPP_FILES! -o %OUTPUT_FILE%
