# Palworld Object Logger
A very simple mod which logs the GUIDs and locations of any Lifmunk Effigy or Journal Entry upon collection

## How To Install
- Download the latest release from the [releases page]()
- Extract to your root game directory (ie, `C:/SteamLibrary/steamapps/common/Palworld`)
	- **NOTE:** If you already use UE4SS (ie, for other mods), make a backup of `<game_root>/Pal/Binaries/Win64/UE4SS.dll` because this will overwrite it with [a very specific release version](https://github.com/UE4SS-RE/RE-UE4SS/actions/runs/11236266667). Also make sure to disable any mods which have trouble working with this specific version of UE4SS in order to minimize bugs/crashes.
- **OPTIONAL** - For a more visual logging experience, enable the UE4SS GUI console:
	- Open `<game_root>/Pal/Binaries/Win64/UE4SS-settings.ini` in a text editor
	- Change `ConsoleEnabled = 1` to `ConsoleEnabled = 0`
	- Change `GuiConsoleEnabled = 0` to `GuiConsoleEnabled = 1`
	- Change `GuiConsoleVisible = 0` to `GuiConsoleVisible = 1`
	- Optionally change `GraphicsAPI` to either `dx11`, `d3d11` or `opengl` (if you encounter any console rendering issues)
	- In the top-left text box of the Console tab, type `PWObjectLogger` to filter for events coming only from this mod

## How To Use
- Once installed, all you need to do is play the game. Any level objects you obtain will automatically be logged to `<game_root>/Pal/Binaries/Win64/UE4SS.log`, and will begin with `[PWObjectLogger]`.
- **NOTE** that every time you start the game, the contents of `UE4SS.log` gets deleted, so be sure to backup any important log information you gather before relaunching the game again

## How To Build From Source
- You will first need to follow the [build instructions for UE4SS](https://github.com/UE4SS-RE/RE-UE4SS#build-requirements)... However, since this project currently uses commit `b6d995c`, you will need to `git reset --hard b6d995c` before `git submodule update --init --recursive`
	- **NOTE:** If you have any trouble, the [UE4SS dev docs](https://docs.ue4ss.com/dev/) are an invaluable resource
- Once you have UE4SS properly set up, clone this repo into `RE-UE4SS/cppmods`
- Add the line `includes("PWObjectLoggerMod")` to `RE-UE4SS/cppmods/xmake.lua`
- Reconfigure UE4SS (`xmake f -m "Game__Shipping__Win64" -y -v`)
	- **NOTE:** If you have trouble configuring UE4SS, make sure you have your ssh keys properly setup (both on your local machine, and on GitHub, with a local ssh agent properly logged in) and that you have [Unreal Engine GitHub source code](https://www.unrealengine.com/en-US/ue-on-github) access linked on your ssh account
- Create a new MSVC project (`xmake project -k vsxmake2022 -m "Game__Shipping__Win64" -y -v`)
- Open `RE-UE4SS/vsxmake2022/RE-UE4SS.sln`, right-click `mods/PWObjectLoggerMod` and choose `Project Only` > `Build Only PWObjectLoggerMod`
- All binaries can be found in `RE-UE4SS/Binaries/Game__Shipping__Win64/`. You will need to manually copy/rename `UE4SS/UE4SS.dll` and `PWObjectLoggerMod/PWObjectLoggerMod.dll` to `<game_root>/Pal/Binaries/Win64/UE4SS.dll` and `<game_root>/Pal/Binaries/Win64/mods/PWObjectLogger/main.dll`, respectively, and then make sure to enable the mod (ie, via creating an empty `mods/PWObjectLogger/enabled.txt`)