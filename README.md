![license](https://img.shields.io/github/license/fedddddd/h2-mod.svg)
[![open bugs](https://img.shields.io/github/issues/fedddddd/h2-mod/bug?label=bugs)](https://github.com/fedddddd/h2-mod/issues?q=is%3Aissue+is%3Aopen+label%3Abug)


# H2: Mod

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
