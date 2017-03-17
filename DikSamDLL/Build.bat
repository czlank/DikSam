@echo off
cp ../DikSam/Compiler/DikSam.l Compiler/Diksam.l
cp ../DikSam/Compiler/DikSam.y Compiler/Diksam.y
cd Compiler
@echo on
make
@echo off
cd..