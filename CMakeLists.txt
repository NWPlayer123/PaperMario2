cmake_minimum_required(VERSION 3.8)

include(cmake/CodeWarrior.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "cmake/FixExtension.cmake")

project(G8MJ01)

#note to self: __declspec(section ".sdata")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") #-w all -w nonotused
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}") #TODO: add more?
#set(CMAKE_C_FLAGS_RELEASE "-O0 -opt nodeadcode -sdata 48")
set(CMAKE_C_FLAGS_RELEASE "-Op -O4 -opt nodeadcode -sdata 48 -w off")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -w all
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") #TODO: add more?
#set(CMAKE_CXX_FLAGS_RELEASE "-O0 -opt nodeadcode -sdata 48")
set(CMAKE_CXX_FLAGS_RELEASE "-Op -O4 -opt nodeadcode -sdata 48 -w off") #sdata 0x30 based on memory.c size_table, TODO: update
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")

include_directories(${SDK_INC_PATH})
include_directories(include)

add_executable(${PROJECT_NAME}.elf
	#source/dvdmgr.c
	source/main.c
	source/mariost.c
	source/mario_pouch.c
	source/memory.c
	source/pmario_sound.c
	source/seq_title.c
	source/sound.c
	source/system.c

	source/demo/DEMOInit.c

	source/drv/mapdrv.c
	source/drv/msgdrv.c
	source/drv/swdrv.c

	source/evt/evt_badgeshop.c

	source/evtmgr/evtmgr.c
	source/evtmgr/evtmgr_cmd.c

	source/filemgr/filemgr.c
	)