# Paper Mario: The Thousand-Year Door Decompilation Project
## Introduction
This is an attempt to decompile/rewrite back to source code Paper Mario: The Thousand-Year Door (Japanese version, G8MJ01), based on symbol maps from the Paper Mario 2 demo. It does not try to be a 1:1 recompile of the dol/rels, but gets as close as possible without changing code-flow/struct layout.

If you find this project useful, [I'd](https://github.com/NWPlayer123) really appreciate a thank you or a tip/donation (looking at you, Nintendo, should you choose to use this codebase for some future remaster/spin-off project).
## How to Compile
To compile, you must set a couple environment variables on Windows.

* `MW_BASE_PATH`: Path to a CodeWarrior 2.7 for GameCube installation (root has `PowerPC_EABI_Support` / `PowerPC_EABI_Tools`).
* `SDK_BASE_PATH`: Path to a Nintendo GameCube SDK installation (root has `HW2` / `include`).
* `MUSYX_BASE_PATH`: Path to a MusyX 2.0 SDK installation (root has `HW2` / `include`).

Then, you should just be able to run build.py to compile the project. If there are no compilation errors (which can be common when working on decompiling/rewriting a game), you should get a `.elf` and `.map` with the compiled code.
## License
This project is distributed under the MIT license. By contributing to this project, you agree to license your contribution under the MIT license to this project.

The copyright notice is currently under [NWPlayer123](https://github.com/NWPlayer123), although that may need to change in the future (to e.g. "The TTYD Decompilation Team") if enough outside contributions are made. By contributing to this project, you also agree to this potential copyright/license change, should it need to happen.
## Credits
Decompilation, Build System by **[NWPlayer123](https://github.com/NWPlayer123)**

Additional Help, Previous Work by **[PistonMiner](https://github.com/PistonMiner)**, **[Jdaster64](https://github.com/jdaster64)**, **[SeekyCt](https://github.com/SeekyCt)**, **[JasperRLZ](https://github.com/magcius)**
