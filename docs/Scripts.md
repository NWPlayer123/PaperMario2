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
<!-- TODO: add details about evtGet/evtSet so I can reference them in the commands section -->
<!-- TODO: just list all the types of work here, and update hyperlinks once evtmgr_cmd is 1:1'd or at least more stable -->

# Commands
## OPCODE_NEXT (0, 0x00)
This opcode is used as an internal fetch that loads the base data on the next opcode and updates all its pointers. Not for use in actual scripts.

## OPCODE_END_SCRIPT (1, 0x01)
This is the final opcode in all scripts, mainly used internally for searching with nesting opcodes (if, else, case, while, label). All scripts must end with this opcode.

## OPCODE_END_EVENT (2, 0x02)
This opcode is used in conjunction with [END_SCRIPT](#opcode_end_script-1-0x01). This cleans up all child and brother events, along with the current event in order to finish execution. This opcode acts as a `return;`, and there can be multiple inside a script.

## OPCODE_LABEL (3, 0x03)
This defines a label in an internal jump table that can store up to 16 labels. It can be used in conjunction with [GOTO](#opcode_goto-4-0x04) for arbitrary jumps in code flow. Takes one argument, which it will use as the identifier to allow goto to find a specific label.

## OPCODE_GOTO (4, 0x04)
This opcode reads from evtGetValue and uses the value to look for a [LABEL](#opcode_label-3-0x03) with the same identifier, and then "jumps" to it. Continues execution at the same location if no label is found.

## OPCODE_DO (5, 0x05)
This opcode is used with [WHILE](#opcode_while-6-0x06) in order to do basic loops. This signifies the start of the loop. Takes a single argument for number of loops to do, which can either use an evtGetValue entry if it's below -10M, or will use its internal counter. If it's set to 0, it will loop infinitely until you use [DO_BREAK](#opcode_do_break-7-0x07).

## OPCODE_WHILE (6, 0x06)
This opcode is used with [DO](#opcode_do-5-0x05) in order to do basic loops. This signifies the end of the loop. See [OPCODE_DO](#opcode_do-5-0x05) for more details on the implementation.

## OPCODE_DO_BREAK (7, 0x07)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `break;` in a loop. It will search until it finds an equivalent WHILE or [END_SCRIPT](#opcode_end_script-1-0x01), and continues execution from the opcode after that.

## OPCODE_DO_CONTINUE (8, 0x08)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `continue;` in a loop. It will search until it finds an equivalent WHILE or [END_SCRIPT](#opcode_end_script-1-0x01), and continues execution at that opcode (i.e. it will execute the WHILE, updating the counter and continuing to loop).




