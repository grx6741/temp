# Handle script arguments (MUST be at the top)
param (
    [string]$Task = "all"
)

# Set paths
$src = Get-ChildItem -Path "src" -Recurse -Filter "*.cpp" | ForEach-Object { $_.FullName }
$bin = "bin\game.html"
$lib = ".\lib\libraylib.a -L.\lib"
$inc = "-I.\include " + (Get-ChildItem -Path "src" -Directory -Recurse | ForEach-Object { "-I" + $_.FullName }) -join " "
$pch = ".\include\pch.h"
$pch_o = ".\include\pch.h.gch"

# Compilation flags
$emflags = @(
    "-Os",
    "-s USE_GLFW=3",
    "-s FETCH=1",
    "-s INITIAL_MEMORY=64MB",
    "-s MAXIMUM_MEMORY=256MB",
    "-s ALLOW_MEMORY_GROWTH=1",
    "--preload-file assets",
    "-pthread"
) -join " "

$cppflags = "-std=gnu++17 -MJ compile_commands.json"

# Create bin directory if it doesn't exist
function Ensure-BinDirectory {
    if (-not (Test-Path "bin")) {
        Write-Host "Creating bin directory..."
        New-Item -ItemType Directory -Path "bin" | Out-Null
    }
}

# Precompiled header
function Build-PCH {
    Write-Host "Building precompiled header..."
    Ensure-BinDirectory
    $cmd = "emcc $emflags -x c++-header $cppflags $pch -o $pch_o"
    Invoke-Expression $cmd
}

# Compile source files
function Build-Project {
    Write-Host "Building project..."
    Ensure-BinDirectory
    $cmd = "emcc -o $bin $src $cppflags $lib $inc $emflags --shell-file .\src\shell.html -DPLATFORM_WEB -include $pch"
    Invoke-Expression $cmd
}

# Clean build files
function Clean {
    Write-Host "Cleaning build files..."
    Remove-Item -Path "bin\*" -Force -ErrorAction SilentlyContinue
    Remove-Item -Path $pch_o -Force -ErrorAction SilentlyContinue
    Remove-Item -Path "bin" -Recurse -Force -ErrorAction SilentlyContinue
}

# Run the game
function Run-Game {
    Write-Host "Running game..."
    Invoke-Expression "emrun $bin"
}

# Handle script arguments
switch ($Task) {
    "clean" { Clean }
    "pch" { Build-PCH }
    "build" { Build-Project }
    "run" { Run-Game }
    "all" { Clean; Build-PCH; Build-Project }
    default { Write-Host "Usage: .\build.ps1 [clean|pch|build|run|all]" }
}
