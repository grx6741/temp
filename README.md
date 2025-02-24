# HELP ME 😭

The game is written in C++ with Raylib and its built only for web with Emscripten.

## Build Instructions

Clone with all the submodules

```console
git clone https://github.com/grx6741/temp --recursive
```

`emsdk` will also be cloned in the repo. This command will install the Emscripten compiler with other deps

```console
.\emsdk\emsdk install latest
```

Now to build the project

```console
# Windows Powershell
.\emsdk\emsdk activate latest
.\build.ps1 all
```

Now to view the game

```console
emrun bin/game.html
```