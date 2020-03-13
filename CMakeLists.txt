cmake_minimum_required(VERSION 3.8)

include(cmake/CodeWarrior.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "cmake/FixExtension.cmake")

project(G8MJ01)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") #-w all -w nonotused
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}") #TODO: add more?
set(CMAKE_C_FLAGS_RELEASE "-Op -O4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -w all
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") #TODO: add more?
set(CMAKE_CXX_FLAGS_RELEASE "-Op -O4")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")

include_directories(${SDK_INC_PATH})
include_directories(include)

add_executable(${PROJECT_NAME}.elf
	source/demo/DEMOInit.c

	source/dvdmgr.c
	source/main.c
	source/mariost.c
	source/memory.c
	source/pmario_sound.c
	source/seq_title.c
	source/sound.c
)