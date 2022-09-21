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


<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->
# Commands
## OPCODE_NEXT (0, 0x00)
This opcode is used as an internal fetch that loads the base data on the next opcode and updates all its pointers. Not for use in actual scripts.

## OPCODE_END_SCRIPT (1, 0x01)
This is the final opcode in all scripts, mainly used internally for searching with nesting opcodes (if, else, case, while, label). All scripts must end with this opcode.

## OPCODE_END_EVENT (2, 0x02)
This opcode is used in conjunction with [END_SCRIPT](#opcode_end_script-1-0x01). This cleans up all child and brother events, along with the current event in order to finish execution. It will also copy any needed data to the parent waiting on it (see [evtDelete](../source/mgr/evtmgr.c#L496)). This opcode acts as a `return;`, and there can be multiple inside a script.

## OPCODE_LABEL (3, 0x03)
This defines a label in an internal jump table that can store up to 16 labels. It can be used in conjunction with [GOTO](#opcode_goto-4-0x04) for arbitrary jumps in code flow. Takes one argument, which it will use as the identifier to allow goto to find a specific label. Only the first byte is read, meaning the only valid identifiers are 0-0xFF.

## OPCODE_GOTO (4, 0x04)
This opcode reads from evtGetValue and uses the value to look for a [LABEL](#opcode_label-3-0x03) with the same identifier, and then "jumps" to it. Continues execution at the same location if no label is found.

Note that using 0xFF in a goto without a corresponding label will cause the game to crash as the next opcode address will be 0, which is invalid memory.

## OPCODE_DO (5, 0x05)
This opcode is used with [WHILE](#opcode_while-6-0x06) in order to do basic loops. This signifies the start of the loop. Takes a single argument for number of loops, which can either be 0 to allow for an infinite loop (see [DO_BREAK](#opcode_do_break-7-0x07)), a normal counter, or if it's below -10M, it will use evtGetValue as a counter. This increases nesting depth, and sets up loop data.

Note that using \[-1, -30M+16\] will cause unintended behavior, as it will decrement until it hits \[-20M, -30M+16], at which point it will crash once it tries to read invalid memory. See implementation of evtGetValue for more details.

## OPCODE_WHILE (6, 0x06)
This opcode is used with [DO](#opcode_do-5-0x05) in order to do basic loops. This signifies the end of the loop. See [OPCODE_DO](#opcode_do-5-0x05) for more details on the implementation. Once it is finished looping, it will decrease nesting depth.

## OPCODE_DO_BREAK (7, 0x07)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `break;` in a loop. It will search until it finds an equivalent WHILE or [END_SCRIPT](#opcode_end_script-1-0x01), decrease nesting depth, and continue execution from the opcode after that (i.e. next cycle it will execute the instruction *after* WHILE/END_SCRIPT).

Note that this may cause unintended behavior as END_SCRIPT is meant to be the final opcode in a script, so it can arbitrary try to execute whatever data is after that opcode.

## OPCODE_DO_CONTINUE (8, 0x08)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `continue;` in a loop. It will search until it finds an equivalent WHILE or [END_SCRIPT](#opcode_end_script-1-0x01), and continues execution at that opcode (i.e. next cycle it will execute the WHILE, updating the counter and continuing to loop).

## OPCODE_WAIT_FRAME (9, 0x09)
This opcode, as the name implies, blocks the script from continuing execution until a set number of frames has passed. Takes a single argument from evtGetValue, which is the number of frames to wait.

Note that this assumes that the event has a speed/timescale of 1, and is executing 1 command every frame. If it's faster or slower, this opcode will be too, since it will be ran multiple times per frame.

## OPCODE_WAIT_MSEC (10, 0x0A)
This opcode, as the name implies, blocks the script from continuing execution until a set amount of time has passed. Takes a single argument from evtGetValue, which is the amount of milliseconds to wait.

Note that this is unaffected by the event's speed/timescale, unlike [WAIT_FRAME](#opcode_wait_frame-9-0x09), so it can be used safely for time-sensitive operations. However, increasing the speed/timescale will decrease the latency until this opcode is finished, since it will check more frequently to see if that amount of time has passed.

## OPCODE_HALT (11, 0xB)
This opcode takes one argument using evtGetValue, and halts execution of the script until that value hits 0. Useful as a way to wait on a bit of data to be valid.

# If/Else Opcodes
## String Comparisons
### OPCODE_IF_STR_EQUAL (12, 0x0C)
This opcode is used for if statements comparing two strings using strcmp. Takes two arguments, the two strings you want to compare, using evtGetValue. If either value is 0, it will treat it as `""`. If they are equal according to strcmp, it will continue execution. If it's not equal, it will search until it finds an equivalent [ELSE](opcode_else-32-0x20), [END_IF](opcode_end_if-33-0x21), or [END_SCRIPT](opcode_end_script-1-0x01), respecting any nested if/else data, and next cycle it will continue execution there.

### OPCODE_IF_STR_NOT_EQUAL (13, 0x0D)
This opcode is used for if statements comparing two strings using strcmp. Equivalent to [IF_STR_EQUAL](opcode_if_str_equal-12-0x0c), continues execution if the strings are not equal, searching for an equivalent else, endif, or end_script if the strings are equal.

### OPCODE_IF_STR_LESS (14, 0x0E)
This opcode is used for if statements comparing two strings using strcmp. Equivalent to [IF_STR_EQUAL](opcode_if_str_equal-12-0x0c), continues execution if string 1 is less than (<) string 2, searching for an equivalent else, endif, or end_script if string 1 is greater than or equal to (>=) string 2.

### OPCODE_IF_STR_GREATER (15, 0x0F)
This opcode is used for if statements comparing two strings using strcmp. Equivalent to [IF_STR_EQUAL](opcode_if_str_equal-12-0x0c), continues execution if string 1 is greater than (>) string 2, searching for an equivalent else, endif, or end_script if string 1 is less than or equal to (<=) string 2.

### OPCODE_IF_STR_LESS_EQUAL (16, 0x10)
This opcode is used for if statements comparing two strings using strcmp. Equivalent to [IF_STR_EQUAL](opcode_if_str_equal-12-0x0c), continues execution if string 1 is less than or equal to (<=) string 2, searching for an equivalent else, endif, or end_script if string 1 is greater than (>) string 2.

### OPCODE_IF_STR_GREATER_EQUAL (17, 0x11)
This opcode is used for if statements comparing two strings using strcmp. Equivalent to [IF_STR_EQUAL](opcode_if_str_equal-12-0x0c), continues execution if string 1 is greater than or equal to (>=) string 2, searching for an equivalent else, endif, or end_script if string 1 is less than (<) string 2.

## Float Comparisons
### OPCODE_IFF_EQUAL (18, 0x12)
This opcode is used for if statements comparing two floats. Takes two arguments, the two floats you want to compare, using evtGetFloat. If they are equal, it will continue execution. If they are not equal, it will search until it finds an equivalent [ELSE](opcode_else-32-0x20), [END_IF](opcode_end_if-33-0x21), or [END_SCRIPT](opcode_end_script-1-0x01), respecting any nested if/else data, and next cycle it will continue execution there.

### OPCODE_IFF_NOT_EQUAL (19, 0x13)
This opcode is used for if statements comparing two floats. Equivalent to [IFF_EQUAL](opcode_iff_equal-18-0x12), continues execution if the floats are not equal, searching for an equivalent else, endif, or end_script if they are equal.

### OPCODE_IFF_LESS (20, 0x14)
This opcode is used for if statements comparing two floats. Equivalent to [IFF_EQUAL](opcode_iff_equal-18-0x12), continues execution if float 1 is less than (<) float 2, searching for an equivalent else, endif, or end_script if float 1 is greater than or equal to (>=) float 2.

### OPCODE_IFF_GREATER (21, 0x15)
This opcode is used for if statements comparing two floats. Equivalent to [IFF_EQUAL](opcode_iff_equal-18-0x12), continues execution if float 1 is greater than (>) float 2, searching for an equivalent else, endif, or end_script if float 1 is less than or equal to (<=) float 2.

### OPCODE_IFF_LESS_EQUAL (22, 0x16)
This opcode is used for if statements comparing two floats. Equivalent to [IFF_EQUAL](opcode_iff_equal-18-0x12), continues execution if float 1 is less than or equal to (<=) float 2, searching for an equivalent else, endif, or end_script if float 1 is greater than (>) float 2.

### OPCODE_IFF_GREATER_EQUAL (23, 0x17)
This opcode is used for if statements comparing two floats. Equivalent to [IFF_EQUAL](opcode_iff_equal-18-0x12), continues execution if float 1 is greater than or equal to (>=) float 2, searching for an equivalent else, endif, or end_script if float 1 is less than (<) float 2.

## Integer Comparisons
### OPCODE_IF_EQUAL (24, 0x18)
This opcode is used for if statements comparing two signed integers. Takes two arguments, the two integers you want to compare, using evtGetValue. If they are equal, it will continue execution. If they are not equal, it will search until it finds an equivalent [ELSE](opcode_else-32-0x20), [END_IF](opcode_end_if-33-0x21), or [END_SCRIPT](opcode_end_script-1-0x01), respecting any nested if/else data, and next cycle it will continue execution there.

### OPCODE_IF_NOT_EQUAL (25, 0x19)
This opcode is used for if statements comparing two integers. Equivalent to [IF_EQUAL](opcode_if_equal-24-0x18), continues execution if the integers are not equal, searching for an equivalent else, endif, or end_script if they are equal.

### OPCODE_IF_LESS (26, 0x1A)
This opcode is used for if statements comparing two integers. Equivalent to [IF_EQUAL](opcode_if_equal-24-0x18), continues execution if integer 1 is less than (<) integer 2, searching for an equivalent else, endif, or end_script if integer 1 is greater than or equal to (>=) integer 2.

### OPCODE_IF_GREATER (27, 0x1B)
This opcode is used for if statements comparing two integers. Equivalent to [IF_EQUAL](opcode_if_equal-24-0x18), continues execution if integer 1 is greater than (>) integer 2, searching for an equivalent else, endif, or end_script if integer 1 is less than or equal to (<=) integer 2.

### OPCODE_IF_LESS_EQUAL (28, 0x1C)
This opcode is used for if statements comparing two integers. Equivalent to [IF_EQUAL](opcode_if_equal-24-0x18), continues execution if integer 1 is less than or equal to (<=) integer 2, searching for an equivalent else, endif, or end_script if integer 1 is greater than (>) integer 2.

### OPCODE_IF_GREATER_EQUAL (29, 0x1D)
This opcode is used for if statements comparing two integers. Equivalent to [IF_EQUAL](opcode_if_equal-24-0x18), continues execution if integer 1 is greater than or equal to (>=) integer 2, searching for an equivalent else, endif, or end_script if integer 1 is less than (<) integer 2.

## Flag Comparisons
### OPCODE_IF_FLAG (30, 0x1E)
This opcode is used to check if a value has certain bits/flags set. Takes two arguments, a signed integer using evtGetValue which is what you want to check, and a signed integer directly from arguments, which are the bits you want checked.

If `value & flags` does not equal zero (any of the bits are set), it will continue execution. If it equals zero, it will search until it finds an equivalent [ELSE](opcode_else-32-0x20), [END_IF](opcode_end_if-33-0x21), or [END_SCRIPT](opcode_end_script-1-0x01), respecting any nested if/else data, and next cycle it will continue execution there.

### OPCODE_IF_NOT_FLAG (31, 0x1F)
This opcode is used to check if a value does not have certain bits/flags set. Equivalent to [IF_FLAG](opcode_if_flag-30-0x1e), continues execution if `value & flags` equals zero (none of the bits are set), searching for an equivalent else, endif, or end_script if any of the bits are set.

## Control Flow
### OPCODE_ELSE (32, 0x20)
This opcode is used as the "else" case for all of the above string, float, and integer comparison opcodes. 
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

### OPCODE_END_IF (33, 0x21)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

# Switch Opcodes
## Headers
### OPCODE_SWITCH (34, 0x22)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SWITCHI (35, 0x23)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_EQUAL (36, 0x24)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_NOT_EQUAL (37, 0x25)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_LESS (38, 0x26)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_GREATER (39, 0x27)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_LESS_EQUAL (40, 0x28)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_GREATER_EQUAL (41, 0x29)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_ETC (42, 0x2A)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_OR (43, 0x2B)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_AND (44, 0x2C)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_FLAG (45, 0x2D)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_END (46, 0x2E)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CASE_BETWEEN (47, 0x2F)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SWITCH_BREAK (48, 0x30)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_END_SWITCH (49, 0x31)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET (50, 0x32)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SETI (51, 0x33)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SETF (52, 0x34)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_ADD (53, 0x35)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SUB (54, 0x36)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_MUL (55, 0x37)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DIV (56, 0x38)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_MOD (57, 0x39)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_ADDF (58, 0x3A)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SUBF (59, 0x3B)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_MULF (60, 0x3C)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DIVF (61, 0x3D)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_READ (62, 0x3E)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READ (63, 0x3F)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READ2 (64, 0x40)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READ3 (65, 0x41)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READ4 (66, 0x42)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READ_N (67, 0x43)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_READF (68, 0x44)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READF (69, 0x45)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READF2 (70, 0x46)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READF3 (71, 0x47)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READF4 (72, 0x48)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_READF_N (73, 0x49)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_USER_WORK (74, 0x4A)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_USER_FLAG (75, 0x4B)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_ALLOC_USER_WORK (76, 0x4C)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_AND (77, 0x4D)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_ANDI (78, 0x4E)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_OR (79, 0x4F)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_ORI (80, 0x50)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_FRAME_FROM_MSEC (81, 0x51)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_MSEC_FROM_FRAME (82, 0x52)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_RAM (83, 0x53)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_RAMF (84, 0x54)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_GET_RAM (85, 0x55)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_GET_RAMF (86, 0x56)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SETR (87, 0x57)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SETRF (88, 0x58)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_GETR (89, 0x59)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_GETRF (90, 0x5A)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_USER_FUNC (91, 0x5B)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_RUN_EVENT (92, 0x5C)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_RUN_EVENT_ID (93, 0x5D)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_RUN_CHILD_EVENT (94, 0x5E)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DELETE_EVENT (95, 0x5F)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_RESTART_EVENT (96, 0x60)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_PRI (97, 0x61)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_SPD (98, 0x62)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_SET_TYPE (99, 0x63)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_STOP_ALL (100, 0x64)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_START_ALL (101, 0x65)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_STOP_OTHER (102, 0x66)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_START_OTHER (103, 0x67)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_STOP_ID (104, 0x68)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_START_ID (105, 0x69)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_CHECK_EVENT (106, 0x6A)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_INLINE_EVENT (107, 0x6B)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_INLINE_EVENT_ID (108, 0x6C)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_END_INLINE (109, 0x6D)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_BROTHER_EVENT (110, 0x6E)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_BROTHER_EVENT_ID (111, 0x6F)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_END_BROTHER (112, 0x70)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_PUT_MSG (113, 0x71)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_MSG_CLEAR (114, 0x72)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_PUT_REG (115, 0x73)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_NAME (116, 0x74)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_REM (117, 0x75)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->

## OPCODE_DEBUG_BP (118, 0x76)
<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->
