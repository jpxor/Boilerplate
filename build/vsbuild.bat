@echo off

REM EMBER: C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC>vcvarsall.bat amd64

REM store calling dir, move to src dir
set CallDir=%CD%
cd %~dp0\..\src

REM Compile and Link
cl /std:c++14 /EHsc /MD /W4 ^
  @..\build\incpath.txt ^
  @..\build\srcfiles.txt ^
  @..\build\mslibs.txt ^
  /link /NODEFAULTLIB:LIBCMT

REM Clean up
move main.exe ..\bin
del *.obj

REM Run
"../bin/main.exe"

REM reset calling dir
cd %CallDir%

