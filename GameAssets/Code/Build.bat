@ECHO OFF
g++ -g -shared -IEngine/Source Bin/Assets/Code/Test.cpp -IBin/Assets/Code -LBin -lEngine -o Bin/Assets/TestLib.dll