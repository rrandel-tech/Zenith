@echo off
pushd %~dp0

echo.
echo ====================================
echo   Z E N I T H   S E T U P   [*]
echo ------------------------------------
echo   Running initial environment setup
echo ====================================
echo.

where python >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo [!]	Python is not installed or not in PATH.
	echo			Please install Python 3.10+ and try again.
	pause
	exit /b
)

python3 Setup.py
PAUSE