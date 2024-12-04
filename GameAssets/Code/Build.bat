@ECHO OFF

PUSHD Bin
g++ -g -shared ../GameAssets/Code/*.cpp -I../GameAssets/Code -I../Engine/Source -L../Bin -lEngine  -o GameCode.dll
POPD