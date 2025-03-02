$SDL_VERSION = "3.2.4"
$IMGUI_VERSION = "1.91.8"

# Create the "Temp" folder for archives and the "Vendor" folder
# for the libraries to be used
New-Item -Path "." -Name "Temp" -ItemType "directory"
New-Item -Path "." -Name "Vendor" -ItemType "directory"

# Download the libraries
Invoke-WebRequest `
	-Uri "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL3-devel-$SDL_VERSION-VC.zip" `
	-OutFile "Temp/SDL3-devel-$SDL_VERSION-VC.zip"
Invoke-WebRequest `
	-Uri "https://github.com/ocornut/imgui/archive/refs/tags/v$IMGUI_VERSION.zip" `
	-OutFile "Temp/imgui-$IMGUI_VERSION.zip"

# Extract SDL and simplify the folder name
Expand-Archive -Path "Temp/SDL3-devel-$SDL_VERSION-VC.zip" -DestinationPath "Vendor"
Rename-Item -Path "Vendor/SDL3-$SDL_VERSION" -NewName "SDL3"

# Extract and prune ImGui for only needed things
Expand-Archive -Path "Temp/imgui-$IMGUI_VERSION.zip" -DestinationPath "Temp"
New-Item -Path "Vendor" -Name "imgui" -ItemType "directory"
Copy-Item -Path "Temp/imgui-$IMGUI_VERSION/*.cpp" -Destination "Vendor/imgui"
Copy-Item -Path "Temp/imgui-$IMGUI_VERSION/*.h" -Destination "Vendor/imgui"
Copy-Item -Path "Temp/imgui-$IMGUI_VERSION/backends/*sdl*3.cpp" -Destination "Vendor/imgui"
Copy-Item -Path "Temp/imgui-$IMGUI_VERSION/backends/*sdl*3*.h" -Destination "Vendor/imgui"

# Cleanup
Remove-Item -Path "Temp" -Force -Recurse

