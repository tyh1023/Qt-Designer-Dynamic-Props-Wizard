@echo off
REM ---------------------------------------------------------------------
REM install.bat - Install the Designer Dynamic Property Widget wizard
REM into the Qt Creator custom wizards directory on Windows.
REM
REM Usage:
REM   install.bat               Install (overwrites existing)
REM   install.bat /uninstall    Remove the wizard
REM   install.bat /dryrun       Show what would happen, do nothing
REM   install.bat /pause        Always pause before exit
REM   install.bat /nopause      Never pause before exit
REM   install.bat /?            Show this help
REM ---------------------------------------------------------------------

setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "WIZARD_NAME=designer-dynamic-props"
set "SOURCE_DIR=%SCRIPT_DIR%\%WIZARD_NAME%"
set "TARGET_ROOT=%APPDATA%\QtProject\qtcreator\templates\wizards"
set "TARGET_DIR=%TARGET_ROOT%\%WIZARD_NAME%"

REM ---- Detect double-click vs command-line launch ----
REM When launched by double-click, cmd.exe runs the script with /c and
REM the window closes on exit unless we pause. When launched from a
REM command prompt, /c is usually absent.
set "PAUSE_AT_END=0"
echo %cmdcmdline% | findstr /i /c:"/c" >nul
if not errorlevel 1 set "PAUSE_AT_END=1"

REM ---- Parse arguments ----
set "MODE=install"
if /I "%~1"=="/uninstall" set "MODE=uninstall"
if /I "%~1"=="/dryrun"    set "MODE=dryrun"
if /I "%~1"=="/pause"     set "PAUSE_AT_END=1"
if /I "%~1"=="/nopause"   set "PAUSE_AT_END=0"
if /I "%~1"=="/?"         goto :help
if /I "%~1"=="--help"     goto :help

REM ---- Sanity checks ----
if /I not "%MODE%"=="uninstall" (
    if not exist "%SOURCE_DIR%\" (
        echo Error: source directory not found: %SOURCE_DIR%
        echo Make sure the wizard folder "%WIZARD_NAME%" exists next to this script.
        set "EXIT_CODE=1"
        goto :cleanup
    )
    if not exist "%SOURCE_DIR%\wizard.json" (
        echo Error: "%SOURCE_DIR%\wizard.json" not found.
        echo The wizard folder seems incomplete.
        set "EXIT_CODE=1"
        goto :cleanup
    )
)

REM ---- Print plan ----
echo Designer Dynamic Property Widget Wizard
echo   OS          : Windows
echo   Source      : %SOURCE_DIR%
echo   Target      : %TARGET_DIR%
echo.

REM ---- Dry run ----
if /I "%MODE%"=="dryrun" (
    if exist "%TARGET_DIR%\" (
        echo [dry-run] Would remove existing: %TARGET_DIR%
    )
    echo [dry-run] Would copy: %SOURCE_DIR%\ -^> %TARGET_DIR%\
    echo [dry-run] Done. No changes were made.
    set "EXIT_CODE=0"
    goto :cleanup
)

REM ---- Uninstall ----
if /I "%MODE%"=="uninstall" (
    if not exist "%TARGET_DIR%\" (
        echo Nothing to uninstall: %TARGET_DIR% does not exist.
        set "EXIT_CODE=0"
        goto :cleanup
    )
    rmdir /s /q "%TARGET_DIR%"
    echo Removed: %TARGET_DIR%
    echo Restart Qt Creator to complete the uninstall.
    set "EXIT_CODE=0"
    goto :cleanup
)

REM ---- Install ----
if not exist "%TARGET_ROOT%\" (
    mkdir "%TARGET_ROOT%" >nul 2>&1
    if errorlevel 1 (
        echo Error: failed to create directory: %TARGET_ROOT%
        set "EXIT_CODE=1"
        goto :cleanup
    )
)

if exist "%TARGET_DIR%\" (
    echo Existing installation found. Replacing it.
    rmdir /s /q "%TARGET_DIR%"
)

xcopy /E /I /Y /Q "%SOURCE_DIR%" "%TARGET_DIR%" >nul
if errorlevel 1 (
    echo Error: failed to copy files.
    set "EXIT_CODE=1"
    goto :cleanup
)

echo Installed successfully.
echo.
echo Next steps:
echo   1. Restart Qt Creator.
echo   2. Open: File ^> New Project
echo   3. Look for: Qt 4 Designer Custom Widget
echo      in the "Other Project" group:
echo      -^> Qt 4 Designer Custom Widget with Dynamic Props
echo.
echo To uninstall later, run: install.bat /uninstall
set "EXIT_CODE=0"
goto :cleanup

REM ---- Help ----
:help
echo install.bat - Install the Designer Dynamic Property Widget wizard.
echo.
echo Usage:
echo   install.bat               Install (overwrites existing)
echo   install.bat /uninstall    Remove the wizard
echo   install.bat /dryrun       Show what would happen, do nothing
echo   install.bat /pause        Always pause before exit
echo   install.bat /nopause      Never pause before exit
set "EXIT_CODE=0"
goto :cleanup

REM ---- Cleanup: pause if needed, then exit ----
:cleanup
if "%PAUSE_AT_END%"=="1" (
    echo.
    pause
)
endlocal & exit /b %EXIT_CODE%