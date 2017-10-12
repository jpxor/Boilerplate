@echo off

REM store calling dir, move to src dir
set CallDir=%CD%
cd %~dp0\..\src

REM Compile and Link
cl /EHsc /MD ^
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

