echo off
cd bin
for /f "delims=" %%# in  ('"wmic path Win32_VideoController  get CurrentHorizontalResolution,CurrentVerticalResolution /format:value"') do (
  set "%%#">nul
)
Vincula.exe r %CurrentHorizontalResolution% %CurrentVerticalResolution% REM windows
pause