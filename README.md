# Paper Mario: The Thousand-Year Door Decompilation Project
## Introduction
This is an attempt to decompile/rewrite back to source code Paper Mario: The Thousand-Year Door (Japanese version, G8MJ01), based on symbol maps from the Paper Mario 2 demo. It does not try to be a 1:1 recompile of the dol/rels, but gets as close as possible without changing code-flow/struct layout.
## How to Compile
To compile, you must set a couple environment variables on Windows.

* `MW_BASE_PATH`: Path to a CodeWarrior for GameCube installation (root has `PowerPC_EABI_Support` / `PowerPC_EABI_Tools`).
* `SDK_BASE_PATH`: Path to a Nintendo GameCube SDK installation (root has `HW2` / `include`).

Then, whenever you want to compile, you have to run /cmake/runme.bat, and then cd to /build and run `cmake -DCMAKE_BUILD_TYPE=Release ..`, with an optional generator e.g. `-G "MSYS Makefiles"`, and then run `make`.

If there are no compilation errors (which can be common when working on decompiling/rewriting a game), you should get a `.elf` and `.map` with the compiled code.
## Credits
Decompilation, Metrowerks Toolchain by **[NWPlayer123](https://github.com/NWPlayer123)**

Additional Help, Previous Work by **[PistonMiner](https://github.com/PistonMiner)**, **[Jdaster64](https://github.com/jdaster64)**, **[SeekyCt](https://github.com/SeekyCt)**, **[JasperRLZ](https://github.com/magcius)**
