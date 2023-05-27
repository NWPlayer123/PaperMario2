# Paper Mario: The Thousand-Year Door Decompilation Project
## Introduction
This repository contains work-in-progress source code decompiled from Paper Mario: The Thousand-Year Door (Japanese version, G8MJ01), based on symbol maps from the Paper Mario 2 demo. It does not currently try to match 1:1, but contains the same functionality.

Decompilation is the process of looking at the compiled bytecode for a program and writing code that produces the same bytecode, thus recovering some semblance of the original program (with originality, there is no way to know if it is actually "correct").
## How to Compile
The build system requires a couple environment variables to be set for various tools that are needed to compile the project, these are:

* `SDK_BASE_PATH`: Path to a 20-Apr-2004 Patch 1 Nintendo GameCube SDK installation (root has `HW2` / `include`, from May 31, 2004).
* `MUSYX_BASE_PATH`: Path to a MusyX 2.0 Patch 3 SDK installation (root has `HW2` / `include`, from August 20, 2004).
* `MW_BASE_PATH`: Path to a CodeWarrior 2.7 for GameCube installation (root has `PowerPC_EABI_Support` / `PowerPC_EABI_Tools`, from August 24, 2004).

Then, you need to install Python 3, run `pip install regex colorama` to install dependencies, and then you can run `build.py` to compile the project. If there are no errors (which can be common while decompiling a program), you should get `build/G8MJ01.elf` and `build/G8MJ01.map` with the compiled code.
## How to Contribute
Currently, the project does not have an easy way to load all symbols needed for decompilation into a single binary. You will need to compile [the doldecomp repo](https://github.com/doldecomp/ttyd) to get an elf that matches retail with function names, which you can load into IDA or Ghidra.

For variable names, the only way is to acquire a copy of the Paper Mario 2 Demo TGC from Demo Disc 2004.7.1/2004.8.1/2004.9.1 for the symbol maps that were used to port to retail.
## License
This project is distributed under the MIT license. By contributing to this project, you agree to license your contribution under the MIT license to this project.

The copyright notice is currently under [NWPlayer123](https://github.com/NWPlayer123), although that may need to change in the future (to e.g. "The TTYD Decompilation Team") if enough outside contributions are made. By contributing to this project, you also agree to this potential copyright/license change, should it need to happen.
## Credits
Decompilation, Build System by **[NWPlayer123](https://github.com/NWPlayer123)**

Additional Help, Previous Work by **[PistonMiner](https://github.com/PistonMiner)**, **[Jdaster64](https://github.com/jdaster64)**, **[SeekyCt](https://github.com/SeekyCt)**, **[JasperRLZ](https://github.com/magcius)**
