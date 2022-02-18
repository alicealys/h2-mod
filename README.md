![license](https://img.shields.io/github/license/fedddddd/h2-mod.svg)
[![open bugs](https://img.shields.io/github/issues/fedddddd/h2-mod/bug?label=bugs)](https://github.com/fedddddd/h2-mod/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
[![Build](https://github.com/fedddddd/h2-mod/workflows/Build/badge.svg)](https://github.com/fedddddd/h2-mod/actions)
[![Build status](https://ci.appveyor.com/api/projects/status/0sh80kdnsvm53rno?svg=true)](https://ci.appveyor.com/project/fedddddd/h2-mod)


# H2: Mod
<p align="center">
  <img src="assets/github/banner.png?raw=true" />
</p>

<br/>

## Download

**NOTE**: Cracked/Pirated versions of the game are NOT compatible with this mod, if you run such a version and have issues/crashes launching the client refer to [this issue](https://github.com/fedddddd/h2-mod/issues/111).

- **[Click here to get the latest release](https://ci.appveyor.com/api/projects/fedddddd/h2-mod/artifacts/build%2Fbin%2Fx64%2FRelease%2Fh2-mod.exe?branch=develop&job=Environment%3A%20APPVEYOR_BUILD_WORKER_IMAGE%3DVisual%20Studio%202022%2C%20PREMAKE_ACTION%3Dvs2022%2C%20CI%3D1%3B%20Configuration%3A%20Release)**
- **You will need to drop this in your Call of Duty: Modern Warfare 2 Campaign Remastered installation folder. If you don't have Call of Duty: Modern Warfare 2 Campaign Remastered, get those game files first.**

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Update the submodules and run `premake5 vs2019` or simply use the delivered `generate.bat`.
- Build via solution file in `build\h2-mod.sln`.

### Premake arguments

| Argument                    | Description                                    |
|:----------------------------|:-----------------------------------------------|
| `--copy-to=PATH`            | Optional, copy the EXE to a custom folder after build, define the path here if wanted. |
| `--dev-build`               | Enable development builds of the client. |

<br/>

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
