@ECHO OFF

PUSHD Bin
g++ -g -shared ../GameCode/*.cpp -I../GameCode -I../Engine/Source -L../Bin -lEngine  -o GameCode.dll
POPD