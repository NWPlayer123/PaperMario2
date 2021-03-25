# Introduction
As with most subsystems, effects are handled through effdrv (effect driver), which keeps track of all effects and handles most of the processing so it's easier to customize individual effects.

# Process Flow
All effect data is centralized in effdrv's work struct. It gets initialized at game boot with `marioStInit` calling `effInit`, which does its own setup along with calling `effInit64`. It allocates 0x100 entries and stores the pointer to that array in its struct.

Any time an effect is needed, you call `effEntry`, which selects an unused entry, does some basic initialization, and gives you a pointer to that entry. effdrv keeps track of which ones are in use with flags (0 = available). Then, that effect adds its own data to the struct which is still contained in the array.

Every tick, `marioStMain` calls `effMain`, which calls the Main() function of any valid effect to do its rendering. Once it's done, it needs to call `effDelete` to free up that slot.

# Customization
An effect can allocate its own userdata to keep track of values from tick to tick with `__memAlloc`, which `effDelete` will deallocate when it's called.
