#!/bin/bash
echo "Making FlashySlideshows.."
 

 
cd InputParser
./make
cd ..


cd AFScripts
./make
cd ..

cd TextAnalyzer
./make
cd ..

LIBS="InputParser/libInputParser.a InputParser/libInputParser_C.a AFScripts/libAFScripts.a TextAnalyzer/libTextAnalyzer.a"

g++ WebSniperApp.cpp WebSniperMain.cpp HTMLAnalyzer.cpp GenerateReport.cpp $LIBS `wx-config --libs` `wx-config --cxxflags` -L. -o bin/Release/WebSniper

cp bin/Release/WebSniper WebSniper
 

echo "Done.."

exit 0
