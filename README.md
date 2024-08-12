![license](https://img.shields.io/github/license/fedddddd/h2-mod.svg)
[![open bugs](https://img.shields.io/github/issues/fedddddd/h2-mod/bug?label=bugs)](https://github.com/fedddddd/h2-mod/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
[![Build](https://github.com/fedddddd/h2-mod/workflows/Build/badge.svg)](https://github.com/fedddddd/h2-mod/actions)
[![Discord](https://img.shields.io/discord/955362057581129738?color=%237289DA&label=members&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/dpnRn2tKT9)
[![Revolt](https://img.shields.io/badge/Revolt.chat-ff4655?style=for-the-badge&logo=Revolt.chat&logoColor=white)](https://rvlt.gg/24n6PDEw)

# H2: Mod
<p align="center">
  <img src="assets/github/banner.png?raw=true" />
</p>

<br/>

## Installation

**NOTE**: You must legally own Call of Duty®: Modern Warfare® 2 Campaign Remastered to run this mod. Cracked/Pirated versions of the game are **NOT** supported.

1. Download the latest **[release](https://h2-mod.fed.cat/data/h2-mod.exe)** or **[develop](https://h2-mod.fed.cat/data-dev/h2-mod.exe)** build
2. Drop the file in your **Call of Duty: Modern Warfare 2 Campaign Remastered** installation folder.  
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
