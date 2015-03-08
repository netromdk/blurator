:: makedist.bat <bin dir> <dist dir> <zip dist>

@echo off
SET "bin_dir=%~1"
SET "dist_dir=%~2"
SET "zip_dist=%~3"

:: Convert paths if necessary!
SETLOCAL EnableDelayedExpansion
SET bin_dir=!bin_dir:/=\!
SET bin_dir=!bin_dir:\\=\!
SET dist_dir=!dist_dir:/=\!
SET dist_dir=!dist_dir:\\=\!
SET zip_dist=!zip_dist:/=\!
SET zip_dist=!zip_dist:\\=\!

echo "!bin_dir!"
echo "!dist_dir!"
echo "!zip_dist!"

:: Copy files/folders
echo "Copying files.."
rmdir /S /Q "!dist_dir!"
mkdir "!dist_dir!" 
xcopy /E "!bin_dir!" "!dist_dir!\"

:: Zip the contents
echo "Zipping contents.."
cd "!dist_dir!\.."
jar cfM dist.zip "%~nx2"
del /F /Q "!zip_dist!"
xcopy dist.zip "!zip_dist!*"
del /F /Q dist.zip

rmdir /S /Q "!dist_dir!"

ENDLOCAL