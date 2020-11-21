@echo off

set VCVARSALL="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
if not defined DevEnvDir call %VCVARSALL%

mingw32-make %*