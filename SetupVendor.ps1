$SDL_VERSION = "3.2.4"

# Create the "Temp" folder for archives and the "Vendor" folder
# for the libraries to be used
New-Item -Path "." -Name "Temp" -ItemType "directory"
New-Item -Path "." -Name "Vendor" -ItemType "directory"

# Download the libraries
Invoke-WebRequest `
	-Uri "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL3-devel-$SDL_VERSION-VC.zip" `
	-OutFile "Temp/SDL3-devel-$SDL_VERSION-VC.zip"

# Extract SDL and simplify the folder name
Expand-Archive -Path "Temp/SDL3-devel-$SDL_VERSION-VC.zip" -DestinationPath "Vendor"
Rename-Item -Path "Vendor/SDL3-$SDL_VERSION" -NewName "SDL3"

# Cleanup
Remove-Item -Path "Temp" -Force -Recurse

