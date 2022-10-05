# Introduction
Paper Mario: The Thousand-Year Door uses a custom scripting system, originally from [Paper Mario (N64)](https://github.com/pmret/papermario/blob/master/include/evt.h) and [updated with many more opcodes](https://github.com/NWPlayer123/PaperMario2/blob/master/include/mgr/evtmgr_cmd.h), in order to allow more developers to work on the game without requiring knowledge of C.

This was done by creating a long list of "commands" with specific opcodes to replicate most of the functionality of C, such as jumps and basic loops, if/else statements, switch statements, waiting for an amount of time or on some value, operators like add/subtract/multiply/divide/modulo, reading/writing data, running other scripts, and calling native C functions (referred to as user functions).

Most of the code that actually runs the game ("events", hence why this is called an event manager) is in this scripting format, which gets compiled to data, with only a small amount of C needed for performance, and to actually run the scripts.

# Technical Details
Every loop in main() (i.e. every frame), [evtmgrMain](../source/mgr/evtmgr.c#L423) will be called, which goes through every "active" event (flags & 1 is set), sorts them according to their priority, and then calls [evtmgrCmd](../source/mgr/evtmgr_cmd.c#L1920) for each script. evtmgrCmd will only return under two conditions: the entire script has finished, or some opcode blocked further commands until the next frame. The only way to influence that is an event's "speed", which will allow evtmgrCmd to be called multiple times per frame, allowing you to continue with a script even if some opcode blocked it. The following opcodes will block evtmgrCmd:
* [END_EVENT](opcode_end_event-2-0x02) cleans up itself and returns that it has finished
* [WAIT_FRAME](opcode_wait_frame-9-0x09) blocks more operations for its entirety (yields after time has passed which also blocks)
* [WAIT_MSEC](opcode_wait_msec-10-0x0a) blocks more operations for its entirety (yields after time has passed which also blocks)
* [HALT](opcode_halt-11-0x0b) blocks more operations only while it's waiting
* [USER_FUNC](opcode_user_func-91-0x5b) is able to return any result, including block, yield, finish, or an error
* [RUN_CHILD_EVENT](opcode_run_child_event-94-0x5e) returns that the event has finished to allow for the child to run
* [END_INLINE](opcode_end_inline-109-0x6d) cleans up itself and returns that it has finished
* [END_BROTHER](opcode_end_brother-112-0x70) blocks further operations this frame, to allow for its brother to continue
* [DEBUG_BP](opcode_debug_bp-118-0x76) yields after it's complete which blocks

This means that only those opcodes are affected by the script speed, as otherwise the entire script will run and complete within a single frame without returning.

# Available Opcodes
<!--
There are [a variety of opcodes](https://github.com/PistonMiner/ttyd-tools/blob/master/ttyd-tools/docs/ttyd-opc-summary.txt) we can use in scripts, including control flow, common operators, comparators, manipulating related scripts, along with the ability to call pre-made C functions, so called user functions, for more complex operations.

There is also the ability to store data, in global or local "saved work" (see [swdrv](https://github.com/NWPlayer123/PaperMario2/blob/master/source/drv/swdrv.c) and evtSet/evtGet functions in [evtmgr_cmd](https://github.com/NWPlayer123/PaperMario2/blob/master/source/mgr/evtmgr_cmd.c)). ..>
<!-- TODO: add details about evtGet/evtSet so I can reference them in the commands section -->
<!-- TODO: just list all the types of work here, and update hyperlinks once evtmgr_cmd is 1:1'd or at least more stable -->


<!-- Note to self: brief overview of the opcode, how many arguments it takes, in-depth technical notes if needed, side effects if abused. -->
# Code Flow Opcodes
### OPCODE_NEXT (0, 0x00)
This opcode is used as an internal fetch that loads the data on the next opcode and updates all its pointers. Not for use in actual scripts.

### OPCODE_END_SCRIPT (1, 0x01)
This opcode is *required* to be the last opcode in all scripts, as it's used to generate jump tables for labels, along with debugging internally when searching with nesting opcodes (if, else, case, while, label).

### OPCODE_END_EVENT (2, 0x02)
This opcode is used in conjunction with [END_SCRIPT](#opcode_end_script-1-0x01), acting as a `return;`, meaning there can be multiple in a single script. This cleans up all child and brother events, along with the current event in order to finish execution. It will also copy any needed data to the parent waiting on it (see [evtDelete](../source/mgr/evtmgr.c#L496)). A script will still run without this opcode, but it will not be freed after it's finished and just idly get called every loop to do nothing once it reaches an END_SCRIPT.

### OPCODE_LABEL (3, 0x03)
This defines a label in an internal jump table that can store up to 16 labels. It can be used in conjunction with [GOTO](#opcode_goto-4-0x04) for arbitrary jumps in code flow. Takes one argument, which it will use as the identifier to allow goto to find a specific label. Only the first byte is read, meaning the only valid identifiers are 0-0xFF.

### OPCODE_GOTO (4, 0x04)
This opcode is used with [LABEL](opcode_label-3-0x03) in order to do arbitrary jumps. It takes one argument using evtGetValue, which is the label identifier. It then uses that value to look for the corresponding label, and continues execution at that location. It will continue at the current location if no label is found.

Note that jumping between nesting depths will cause undefined behavior. Additionally, using 0xFF in a goto without a corresponding label will cause the game to crash as the next opcode address will be 0, which is invalid memory.

There is a check in the search algorithm that allows you to jump to any address and execute it as script code, even outside the bounds of the script. If the signed value passed is lower than -270M, it will simply return it, setting the next command to that value. The GameCube uses 0xC0000000 to 0xC1800000 for uncached access to code and data, which happens to be well represented as a float, namely between -2.0f and -16.0f. This allows for Arbitrary Script Execution (ASE) to be possible, assuming you can manipulate a script to include this data as part of the goto.

# Looping Opcodes
### OPCODE_DO (5, 0x05)
This opcode is used with [WHILE](#opcode_while-6-0x06) in order to do basic loops. This signifies the start of the loop. It takes a single argument for number of loops, which can either be 0 to allow for an infinite loop (see [DO_BREAK](#opcode_do_break-7-0x07)), a positive counter, or if it's below -10M, it will use evtGetValue as a counter. This increases nesting depth, and sets up loop data.

Note that using negative values incorrectly will cause undefined behavior, especially in the \[-1, -10M\) range. See implementation of evtGetValue for more details.

### OPCODE_WHILE (6, 0x06)
This opcode is used with [DO](#opcode_do-5-0x05) in order to do basic loops. This signifies the end of the loop. See [OPCODE_DO](#opcode_do-5-0x05) for more details on the implementation. Once the counter hits 0 (assuming it's not in an infinite loop), it will decrease nesting depth and then continue execution.

### OPCODE_DO_BREAK (7, 0x07)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `break;` in a loop. It will search until it finds an equivalent WHILE, decrease nesting depth, and continue execution from the opcode after that (i.e. next it will execute the instruction *after* WHILE).

Note that trying to break without an equivalent WHILE is undefined behavior, as the search will also accept END_SCRIPT, and will try to execute whatever is *after*.

### OPCODE_DO_CONTINUE (8, 0x08)
This opcode is used with [DO](#opcode_do-5-0x05) and [WHILE](#opcode_while-6-0x06). As the name implies, this acts as a `continue;` in a loop. It will search until it finds an equivalent WHILE, and continues execution at that opcode (i.e. next it will execute the WHILE, updating the counter and continuing to loop).

# Waiting Opcodes
### OPCODE_WAIT_FRAME (9, 0x09)
This opcode, as the name implies, blocks the script from continuing execution until a set number of frames has passed. Takes a single argument, the number of calls (not frames) to block for, using evtGetValue. Every call, it will decrease its internal counter by one, and will only stop blocking once that counter hits zero.

Note that as this is a blocking command, it is influenced by the event's speed. If it is called multiple times per frame, it will finish sooner or later than the actual number of frames passed. See [technical details](#technical-details) for more information on blocking commands.

### OPCODE_WAIT_MSEC (10, 0x0A)
This opcode, as the name implies, blocks the script from continuing execution until a set amount of time has passed. Takes a single argument, the amount of milliseconds to block for, using evtGetValue. Every call, it will check the amount of time that has passed since it was first called, and it will only stop blocking once that amount of milliseconds has passed.

Note that as this is a blocking command, it is influenced by the event's speed. As the time it uses is unaffected, the only thing that speed does is to decrease or increase the latency until the opcode is finished, as it can check multiple times per frame, or once every several frames, instead of only once per frame. See [technical details](#technical-details) for more information on blocking commands.

### OPCODE_HALT (11, 0xB)
This argument blocks the script from continuing execution until some value it is checking is equal to zero. Takes a single argument using evtGetValue, which it will continually check.

Note that as this is a blocking command, it is influenced by the event's speed. The only way it can influence this opcode is to decrease or increase the latency until the opcode is finished, as it can check multiple times per frame, or once every several frames, instead of only once per frame. See [technical details](#technical-details) for more information on blocking commands.

# If/Else Opcodes
## Overview
The way that the search algorithm works in the if/else commands is somewhat unique, in that it relies on putting code execution to *after* the opcodes. If the "if" check passes, it will continue execution, and relies on the "else" opcode" to set the next command to *after* the endif. If the check fails, it sets the next command to *after* the else, and then it continues execution, with the endif acting as a marker and not doing anything if it's actually encountered.

However, all this internal code flow is completely transparent to the scripter. All they need to know is that, if this check passes, do a thing, else do a different thing, and then endif.
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
This opcode is used as the "else" case for all of the above string, float, and integer comparison opcodes. If this opcode is ran, it simply sets the next command to be *after* the endif. See the [overview](#overview-1) for more details.

### OPCODE_END_IF (33, 0x21)
This opcode is used to signify the end of any if/else nesting. It is only used as a marker to help with executing those code blocks, and does not do anything on its own. See the [overview](#overview-1) for more details.

# Switch Opcodes
## Overview
Switch statements work the same as they would in C, along with additional functionality for comparisons, combining multiple checks using OR/AND, checking between a certain range, and checking if certain bits are set.

Additionally, they support a nesting depth up to 8 (separate from [do/while nesting depth](#looping-opcodes)), which is respected by the search algorithms used to find the next case or [END_SWITCH](opcode_end_switch-49-0x31).

When a [SWITCH](opcode_switch-34-0x22) is ran, it increases nesting depth, stores the value being checked, and sets the state to 1. The states work as follows:
* 1 is used as "continue searching for a matching case".
* 0 means that some case statement has matched, and the next case statement it encounters (i.e. once it finishes running the matched code) will then search for END_SWITCH.
* -1 is used for [CASE_OR](opcode_case_or-43-0x2b) and [CASE_AND](opcode_case_and-44-0x2c), in order to designate that "all current checks are valid, continue execution".
* -2 is used in [CASE_AND](opcode_case_and-44-0x2c), in order to say "one of the AND cases failed, continue to the next case", which should be a CASE_END.

CASE_OR and CASE_AND require a [CASE_END](opcode_case_end-46-0x2e) at the end of the code block, in order to reset a -1 or -2 state.

Note that all this internal code flow is completely transparent to the scripter. All they need to do is have a SWITCH statement, any number of cases, including a CASE_END at the end of OR/AND blocks, and then an END_SWITCH, and it will jump to the correct case and run that code.

## Headers
### OPCODE_SWITCH (34, 0x22)
This opcode is used to begin a switch statement, which will use the value provided to jump to the first matching case and run its code block, before jumping to an [END_SWITCH](opcode_end_switch-49-0x31). Takes a single argument, the value you want to compare, using evtGetValue. See the [overview](#overview-2) for more details.

An END_SWITCH is required in order to finish a switch statement.

### OPCODE_SWITCHI (35, 0x23)
This opcode is not accounted for in searching algorithms and should not be used. It is equivalent to [SWITCH](opcode_switch-34-0x22), except it takes the argument directly.

### OPCODE_CASE_EQUAL (36, 0x24)
This opcode is used with [SWITCH](opcode_switch-34-0x22) in order to check if the value is equal to some target value. Takes a single argument, the value you want to compare against, using evtGetValue.

First, it will check if the state is negative or zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if the switch value equals the target value. If it does, it will set the state to 0 and allow the code block to run. Otherwise, it will search for the next case statement and set execution there.

### OPCODE_CASE_NOT_EQUAL (37, 0x25)
This opcode is equivalent to [CASE_EQUAL](opcode_case_equal-36-0x24), except it will check if the switch value is not equal to some target value, searching for the next case statement if it *is* equal.

### OPCODE_CASE_LESS (38, 0x26)
This opcode is equivalent to [CASE_EQUAL](opcode_case_equal-36-0x24), except it will check if the switch value is less than (<) the target value, searching for the next case statement if it is greater than or equal to (>=).

### OPCODE_CASE_GREATER (39, 0x27)
This opcode is equivalent to [CASE_EQUAL](opcode_case_equal-36-0x24), except it will check if the switch value is greater than (>) the target value, searching for the next case statement if it is less than or equal to (<=).

### OPCODE_CASE_LESS_EQUAL (40, 0x28)
This opcode is equivalent to [CASE_EQUAL](opcode_case_equal-36-0x24), except it will check if the switch value is less than or equal to (<=) the target value, searching for the next case statement if it is greater than (>).

### OPCODE_CASE_GREATER_EQUAL (41, 0x29)
This opcode is equivalent to [CASE_EQUAL](opcode_case_equal-36-0x24), except it will check if the switch value is greater than or equal to (>=) the target value, searching for the next case statement if it is less than (<).

### OPCODE_CASE_ETC (42, 0x2A)
This opcode is used with [SWITCH](opcode_switch-34-0x22) as the `default:` case, meant to catch values that do not meet any other cases.

First, it will check if the state is negative or zero, and will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, since it is the catch-all case, it will just set the state to 0 and allow the code block to run.

### OPCODE_CASE_OR (43, 0x2B)
This opcode is used with [SWITCH](opcode_switch-34-0x22), in order to check if *any* of the following compares are valid. Takes a single argument, the target value you want to compare against, using evtGetValue.

First, it will check if the state is zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if the switch value equals the target value. If it does, it will set the state to -1 and continue execution, either to another OR/AND case, or the actual code block. Otherwise, it will check if the state does not equal -1, which will search for the next case (should be a [CASE_END](opcode_case_end-46-0x2e)), and set execution there.

For this opcode to work correctly, the code block it corresponds to must end in an [CASE_END](opcode_case_end-46-0x2e).

### OPCODE_CASE_AND (44, 0x2C)
This opcode is used with [SWITCH](opcode_switch-34-0x22), in order to check if *all* of the following compares are valid. Takes a single argument, the target value you want to compare against, using evtGetValue.

First, it will check if the state is zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if the state is -2, which means a different AND case failed, and will search for the next case (should be a [CASE_END](opcode_case_end-46-0x2e)).

Next, it will check if the switch value equals the target value. If it does, it will set the state to -1 and continue execution, either to another OR/AND case, or the actual code block. Otherwise, it will set the state to -2, and then search for the next case (should be a [CASE_END](opcode_case_end-46-0x2e)) and set execution there.

For this opcode to work correctly, the code block it corresponds to must end in an [CASE_END](opcode_case_end-46-0x2e).

### OPCODE_CASE_FLAG (45, 0x2D)
This opcode is used with [SWITCH](opcode_switch-34-0x22) in order to check if *any* of the flags are set. Takes a single argument, the bitmask you want to compare, directly from arguments.

First, it will check if the state is negative or zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if `(value & flags) != 0`. If any of the bits are set, it will set the state to 0 and allow the code block to run. Otherwise, it will search for the next case statement and set execution there.

### OPCODE_CASE_END (46, 0x2E)
This opcode is used with [CASE_OR](opcode_case_or-43-0x2b) and [CASE_AND](opcode_case_and-44-0x2c) in order to do multi-part checks. It is required at the end of the code block, in order to clean up the state afterwards.

First, it will check if the state is zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if the state is -1, which means all checks passed and this code block ran, and will set the state to 0 and then search for an END_SWITCH. Otherwise, it will reset the state to 1 and search for the next case.

### OPCODE_CASE_BETWEEN (47, 0x2F)
This opcode is used with [SWITCH](opcode_switch-34-0x22) in order to check if the value is inside a given range. Takes two arguments, the minimum and maximum allowed values, using evtGetValue.

First, it will check if the state is negative or zero, which will search for an [END_SWITCH](opcode_end_switch-49-0x31), respecting nesting depth, and set execution there for cleanup. Then, it will check if `min <= value <= max`. If it does, it will set the state to 0 and allow the code block to run. Otherwise, it will search for the next case statement and set execution there.

### OPCODE_SWITCH_BREAK (48, 0x30)
This opcode is used with [SWITCH](opcode_switch-34-0x22), used as a `break;` inside a code block in order to exit the switch statement. It simply searches for an [END_SWITCH](opcode_end_switch-49-0x31), respecting switch nesting depth, and sets execution there for cleanup.

### OPCODE_END_SWITCH (49, 0x31)
This opcode is used with [SWITCH](opcode_switch-34-0x22), and is required to end the switch statement. It will set the state to 0, and then decrease switch nesting depth.

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
