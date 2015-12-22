@ECHO off

REM Set common variables
SET tmpfile=files.tmp
SET bindir=%1\build
SET exefile=%bindir%\%1.exe

REM Check whether specified directory exists.
IF NOT EXIST %1 GOTO notfound

REM Make sure we start with an empty .tmp file
IF EXIST %tmpfile% DEL %tmpfile%

REM Get a list of all .cpp files in the specified directory and save it as temporary file.
setlocal enabledelayedexpansion
FOR %%f IN (%1\*.cpp) DO (
	SET a=%%f
	SET a=!a:\=/!
	ECHO !a! >> files.tmp
)
endlocal

REM Now compile all .cpp files in the temporary directory.
:compile
IF NOT EXIST %tmpfile% GOTO nocxxfile
ECHO Now compiling...
IF NOT EXIST %bindir% MKDIR %bindir%
g++ -DSFML_STATIC -std=c++14 -Os -o %exefile% @%tmpfile% -I%1 -IH:/SFML/include -IH:/lua-5.3.2/include -LH:/SFML/lib/MinGW_510/x64 -LH:/lua-5.3.2/lib -llua -lsfml-graphics-s -lfreetype -ljpeg -lsfml-window-s -lopengl32 -lgdi32 -lsfml-system-s -lwinmm
GOTO success

:notfound
ECHO The specified directory "%1" does not exist.
GOTO end

:nocxxfile
ECHO The specified directory does not contain a .cpp file.
GOTO end

:success
IF EXIST %tmpfile% DEL %tmpfile%
%exefile%
GOTO end

:end
