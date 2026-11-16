@echo off
cd /d "%~dp0"

echo Building...
g++ -std=c++17 *.cpp -o shein.exe
if errorlevel 1 (
    echo.
    echo Build failed. Fix the errors above.
    pause
    exit /b 1
)

echo Build OK. Running...
echo.
shein.exe
echo.
pause