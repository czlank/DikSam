@echo off
if "%1"=="" (
@echo on
cd Compiler
make
cd..
@echo off
) else (
@echo on
cd Compiler
make clean
cd..
@echo off
)