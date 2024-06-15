@echo off

rem Define a script start directory
set SCRIPT_DIR=%~dp0

rem Проверяем наличие make.exe
where mingw32-make.exe >nul 2>nul
if %errorlevel% neq 0 (
    echo make.exe not found. Verify that GNU Make is installed on the system.
    pause
    exit /b 1
)

rem Переходим в директорию проекта
echo %SCRIPT_DIR%
cd "%SCRIPT_DIR%"

rem Выполняем сборку проекта
mingw32-make

rem Проверяем, что сборка прошла успешно
if %errorlevel% neq 0 (
    echo Project Build Error.
    pause
    exit /b 1
)

echo The project was completed successfully.
pause
exit /b 0
