![license](https://img.shields.io/github/license/fedddddd/h2-mod.svg)
[![open bugs](https://img.shields.io/github/issues/fedddddd/h2-mod/bug?label=bugs)](https://github.com/fedddddd/h2-mod/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
[![Build](https://github.com/fedddddd/h2-mod/workflows/Build/badge.svg)](https://github.com/fedddddd/h2-mod/actions)
[![Discord](https://img.shields.io/discord/955362057581129738?color=%237289DA&label=members&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/dpnRn2tKT9)

# H2: Mod
<p align="center">
  <img src="assets/github/banner.png?raw=true" />
</p>

<br/>

## Installation

**NOTE**: Cracked/Pirated versions of the game are **NOT** compatible with this mod, if you run such a version and have issues/crashes when running the client read **Step 2**.

1. Download the latest **[release](https://github.com/fedddddd/h2-mod/releases/latest/download/h2-mod.exe)** or **[develop](https://master.fed0001.xyz/data-dev/h2-mod.exe)** build
2. Drop the file in your **Call of Duty: Modern Warfare 2 Campaign Remastered** installation folder.  
  If you don't have the game installed (or own a cracked/pirated copy of it) you can download it for free from the official **Battle.Net** servers using [Battle.Net Installer](https://github.com/barncastle/Battle.Net-Installer) and executing this command:
  
   ```
   .\bnetinstaller.exe --prod lazr --uid lazarus --lang enUS --dir "YOUR INSTALL PATH"
   ```
   
   **Make sure to replace "YOUR INSTALL PATH" with an actual installation path of your choice.**
3. Run **h2-mod.exe** and make sure you press **"YES"** when asked whether to install updates.

## Common issues

- If you get crashes that show errors like **"Create2DTexture(...) failed ..."** or **"IDXGISwapChain::Present failed: ..."** when loading certain maps try:
  * Disabling shader preloading
  * Lowering graphics settings
  * Freeing up RAM (close programs)
  * Updating your GPU drivers
  
**Do NOT open issues to ask for support!**  
The issues tab is meant for bugs that have to be fixed or features that should be implemented.  
Any issue mentioning things like "h2-mod doesn't work" with no other information included will be closed.

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Update the submodules and run `premake5 vs2019` or simply use the delivered `generate.bat`.
- Build via solution file in `build\h2-mod.sln`.

  ### Premake arguments

  | Argument                    | Description                                    |
  |:----------------------------|:-----------------------------------------------|
  | `--copy-to=PATH`            | Optional, copy the EXE to a custom folder after build, define the path here if wanted. |
  | `--dev-build`               | Enable development builds of the client. |

## Disclaimer

This software has been created purely for the purposes of
academic research. It is not intended to be used to attack
other systems. Project maintainers are not responsible or
liable for misuse of the software. Use responsibly.

## Credits

This project is based on [IW6x](https://github.com/XLabsProject/iw6x-client) and [S1x](https://github.com/XLabsProject/s1x-client)
* [momo5502](https://github.com/momo5502)
* [JariKCoding](https://github.com/JariKCoding/CoDLuaDecompiler)
* [xensik](https://github.com/xensik/gsc-tool/)
* [ZoneTool](https://github.com/ZoneTool/zonetool)
* [quaK](https://github.com/Joelrau)
