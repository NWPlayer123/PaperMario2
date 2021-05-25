cmake_minimum_required(VERSION 3.8)

include(cmake/CodeWarrior.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "cmake/FixExtension.cmake")

project(G8MJ01)

#note to self: __declspec(section ".sdata")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") #-w all -w nonotused
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}") #TODO: add more?
#set(CMAKE_C_FLAGS_RELEASE "-O0 -opt nodead -sdata 48 -sdata2 4 -w off")
set(CMAKE_C_FLAGS_RELEASE "-once -O4,p -opt nodeadcode -sdata 48 -sdata2 6 -multibyte -rostr -use_lmw_stmw on -w all -w nonotused -w nounusedarg -w nopadding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -w all
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") #TODO: add more?
#set(CMAKE_CXX_FLAGS_RELEASE "-O0 -opt nodead -sdata 48 -sdata2 4 -w off")
set(CMAKE_CXX_FLAGS_RELEASE "-once -O4,p -opt nodeadcode -sdata 48 -sdata2 6 -multibyte -rostr -use_lmw_stmw on -w all -w nonotused -w nounusedarg -w nopadding") #sdata 0x30 based on memory.c size_table, TODO: update
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lcf ${PROJECT_SOURCE_DIR}/linker/linker.lcf -map ${PROJECT_NAME}.map")

include_directories(${SDK_INC_PATH})
include_directories(${MW_INC_PATH})
include_directories(${MUSYX_INC_PATH})
include_directories(include)

add_executable(${PROJECT_NAME}.elf
	source/countdown.c
	source/error_handler.c
	source/event.c
	source/item_data.c
	source/main.c
	source/mario.c
	source/mariost.c
	source/mario_cam.c
	source/mario_party.c
	source/mario_pouch.c
	source/memory.c
	source/nameent.c
	source/party.c
	source/pmario_sound.c
	source/romfont.c
	source/sound.c
	source/statuswindow.c
	source/system.c
	source/texPalette.c

	source/battle/battle.c
	source/battle/battle_ac.c
	source/battle/battle_acrobat.c
	source/battle/battle_ac_help.c
	source/battle/battle_audience.c
	source/battle/battle_break_slot.c
	source/battle/battle_camera.c
	source/battle/battle_disp.c
	source/battle/battle_event_cmd.c
	source/battle/battle_icon.c
	source/battle/battle_item_data.c
	source/battle/battle_menu_disp.c
	source/battle/battle_pad.c
	source/battle/battle_seq.c
	source/battle/battle_stage.c
	source/battle/battle_stage_object.c
	source/battle/battle_status_effect.c
	source/battle/battle_status_icon.c
	source/battle/battle_sub.c
	source/battle/battle_unit.c

	source/battle/ac/ac_table.c
	source/battle/ac/ac_timing_a.c

	source/data/npc_data.c

	source/drv/animdrv.c
	source/drv/arcdrv.c
	source/drv/bgdrv.c
	source/drv/camdrv.c
	source/drv/casedrv.c
	source/drv/dispdrv.c
	source/drv/effdrv.c
	source/drv/envdrv.c
	source/drv/extdrv.c
	source/drv/fadedrv.c
	source/drv/hitdrv.c
	source/drv/icondrv.c
	source/drv/imgdrv.c
	source/drv/itemdrv.c
	source/drv/lightdrv.c
	source/drv/mapdrv.c
	source/drv/mobjdrv.c
	source/drv/msgdrv.c
	source/drv/npcdrv.c
	source/drv/offscreendrv.c
	source/drv/seqdrv.c
	source/drv/shadowdrv.c
	source/drv/swdrv.c
	source/drv/windowdrv.c

	source/eff/eff_kemuri.c
	source/eff/eff_n64.c
	source/eff/eff_nice.c
	source/eff/eff_scanning.c
	source/eff/eff_sleep.c

	source/evt/evt_badgeshop.c
	source/evt/evt_eff.c
	source/evt/evt_johoya.c
	source/evt/evt_lottery.c
	source/evt/evt_npc.c
	source/evt/evt_shop.c
	source/evt/evt_yuugijou.c

	source/mgr/arammgr.c
	source/mgr/cardmgr.c
	source/mgr/dvdmgr.c
	source/mgr/evtmgr.c
	source/mgr/evtmgr_cmd.c
	source/mgr/filemgr.c
	source/mgr/fontmgr.c
	source/mgr/winmgr.c

	source/MusyX/seq.c

	source/party/party_vivian.c

	source/sdk/arc.c
	source/sdk/DEMOInit.c

	source/seq/seq_e3.c
	source/seq/seq_mapchange.c
	source/seq/seq_title.c

	source/win/win_main.c
	 "source/data/item_data.c" "include/data/item_data.h")