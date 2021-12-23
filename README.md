# Paper Mario: The Thousand-Year Door Decompilation Project
## Introduction
This is an attempt to decompile/rewrite back to source code Paper Mario: The Thousand-Year Door (Japanese version, G8MJ01), based on symbol maps from the Paper Mario 2 demo. It does not try to be a 1:1 recompile of the dol/rels, but gets as close as possible.

If you find this project useful, [I'd](https://github.com/NWPlayer123) really appreciate a thank you or a tip/donation (looking at you, Nintendo, should you choose to use this codebase for some future remaster/spin-off project).
## How to Compile
To compile, you must set a couple environment variables on Windows.

* `SDK_BASE_PATH`: Path to a 20-Apr-2004 Patch 1 Nintendo GameCube SDK installation (root has `HW2` / `include`, from May 31, 2004).
* `MUSYX_BASE_PATH`: Path to a MusyX 2.0 Patch 3 SDK installation (root has `HW2` / `include`, from August 20, 2004).
* `MW_BASE_PATH`: Path to a CodeWarrior 2.7 for GameCube installation (root has `PowerPC_EABI_Support` / `PowerPC_EABI_Tools`, from August 24, 2004).

Then, you should install Python 3, and run `pip install regex colorama` to install dependencies, and then you can run `build.py` to compile the project. If there are no compilation errors (which can be common when working on decompiling/rewriting a game), you should get a `.elf` and `.map` with the compiled code.
## How to Contribute
The best way to acquire the codebase to work on reverse engineering right now is to get the required assets, compile [the doldecomp repo](https://github.com/doldecomp/ttyd) using the main.dol from the retail disc, and then load the compiled .elf in /build into your reverse engineering tool of choice (IDA, Ghidra).

You'll also have to acquire a copy of the Paper Mario 2 Demo TGC from Demo Disc 2004.7.1/2004.8.1/2004.9.1 for symbols, as the doldecomp repo only covers function names, not variable names.
## License
This project is distributed under the MIT license. By contributing to this project, you agree to license your contribution under the MIT license to this project.

The copyright notice is currently under [NWPlayer123](https://github.com/NWPlayer123), although that may need to change in the future (to e.g. "The TTYD Decompilation Team") if enough outside contributions are made. By contributing to this project, you also agree to this potential copyright/license change, should it need to happen.
## Credits
Decompilation, Build System by **[NWPlayer123](https://github.com/NWPlayer123)**

Additional Help, Previous Work by **[PistonMiner](https://github.com/PistonMiner)**, **[Jdaster64](https://github.com/jdaster64)**, **[SeekyCt](https://github.com/SeekyCt)**, **[JasperRLZ](https://github.com/magcius)**
