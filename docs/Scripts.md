# Introduction
Paper Mario: The Thousand-Year Door uses a custom scripting system, originally from [Paper Mario (N64)](https://github.com/pmret/papermario/blob/master/include/evt.h) and [updated with many more opcodes](https://github.com/NWPlayer123/PaperMario2/blob/master/include/mgr/evtmgr_cmd.h), in order to allow more developers to work on the game's code without requiring knowledge of C. All scripts get linked into data to be interpreted at runtime. It's referred to as "evt", presumably as an Event System.

# Overview
You are able to define scripts using the macros in [evt_cmd](https://github.com/NWPlayer123/PaperMario2/blob/master/include/evt/evt_cmd.h), which will be compiled into data. You can then execute it with any of the evtEntry functions, which will add it to the list of scripts it is currently executing each frame (see [make_pri_table](https://github.com/NWPlayer123/PaperMario2/blob/master/source/mgr/evtmgr.c) for more info), with no further input needed.

It will automatically be cleaned up and freed from execution once it reaches a [RETURN()](https://github.com/NWPlayer123/PaperMario2/blob/master/include/evt/evt_cmd.h) at the end of a script.

# Technical Details
As seen in [evtmgrMain](https://github.com/NWPlayer123/PaperMario2/blob/master/source/mgr/evtmgr.c), which runs each frame/main() game loop, it will run through all active scripts (flags & 1 set), and run as many "commands" as it is allowed this frame, determined by the timescale/speed the script is running at.

It calls [evtmgrCmd()](https://github.com/NWPlayer123/PaperMario2/blob/master/source/mgr/evtmgr_cmd.c) to run a single command, until either we run out of commands this frame, and it must run more in subsequent frames, or we hit the aforementioned RETURN() and it is cleaned up, removing the active flag and allowing a new script to be allocated to that slot.

# Available Opcodes
There are [a variety of opcodes](https://github.com/PistonMiner/ttyd-tools/blob/master/ttyd-tools/docs/ttyd-opc-summary.txt) we can use in scripts, including control flow, common operators, comparators, manipulating related scripts, along with the ability to call pre-made C functions, so called user functions, for more complex operations.

There is also the ability to store data, in global or local "saved work" (see [swdrv](https://github.com/NWPlayer123/PaperMario2/blob/master/source/drv/swdrv.c) and evtSet/evtGet functions in [evtmgr_cmd](https://github.com/NWPlayer123/PaperMario2/blob/master/source/mgr/evtmgr_cmd.c)).
<!-- TODO: just list all the types of work here, and update hyperlinks once evtmgr_cmd is 1:1'd or at least more stable -->
