@ECHO OFF
g++ -g -shared -IEngine/Source Bin/Assets/Code/Test.cpp -IBin/Assets/Code -LBin -lEngine -lBullet -IEngine/Vendor/bullet/bullet3-master/src -o Bin/Assets/TestLib.dll