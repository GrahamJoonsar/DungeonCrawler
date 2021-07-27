@echo off
cls
echo Starting Build
if %%1 == debug g++ -Wall DungeonCrawler.cpp -o DungeonCrawler.exe
if %%1 == release g++ -O3 -static-libstdc++ DungeonCrawler.cpp -o DungeonCrawler.exe
echo Build Complete