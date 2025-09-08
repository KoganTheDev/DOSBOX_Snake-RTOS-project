@echo off
rem This batch file compiles all .c files, deletes .obj files, and runs the main program.

echo Deleting left over .obj files from last runs to force new compilation
del *.obj

echo Compiling all .c files...
rem tcc flags -ml -r are needed for compilation, flag -e + "dosbox~1.exe" makes sure the .exe file name is dosbox~1.exe

tcc -ml -r -edosbox~1.exe -I../include *.c
if errorlevel 1 goto error_compile

echo Deleting .obj files...
del *.obj

echo Running dosbox~1.exe...
dosbox~1.exe

goto end

:error_compile
echo An error occurred during compilation. Please check your source code.

:end
echo.