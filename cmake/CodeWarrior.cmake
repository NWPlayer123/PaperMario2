if(NOT DEFINED ENV{SDK_BASE_PATH})
	message(FATAL_ERROR "SDK_BASE_PATH not defined. Please point it to your SDK root directory.")
endif()
set(SDK_BASE_PATH "$ENV{SDK_BASE_PATH}/")
set(SDK_LIB_PATH "${SDK_BASE_PATH}/HW2/lib/")
set(SDK_INC_PATH "${SDK_BASE_PATH}/include/")

if(NOT DEFINED ENV{MW_BASE_PATH})
	message(FATAL_ERROR "MW_BASE_PATH not defined. Please point it to your Metrowerks CodeWarrior root directory.")
endif()
set(MW_BASE_PATH "$ENV{MW_BASE_PATH}/")
set(MW_LIB_PATH "${MW_BASE_PATH}/PowerPC_EABI_Support/")
set(MW_BIN_PATH "${MW_BASE_PATH}/PowerPC_EABI_Tools/Command_Line_Tools/")
set(MW_INC_PATH "${MW_BASE_PATH}/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/")

if(NOT DEFINED ENV{MUSYX_BASE_PATH})
	message(FATAL_ERROR "MW_BASE_PATH not defined. Please point it to your MusyX root directory.")
endif()
set(MUSYX_BASE_PATH "$ENV{MUSYX_BASE_PATH}/")
set(MUSYX_LIB_PATH "${MUSYX_BASE_PATH}/HW2/lib/")
set(MUSYX_INC_PATH "${MUSYX_BASE_PATH}/include/")

if(NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
	if(NOT DEFINED ENV{MWCIncludes})
		message(FATAL_ERROR "MWCIncludes not defined. Please run runme.bat to set the correct env-var.")
	endif()
endif()
#------------------------------------------------------------------------------------------------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR PowerPC)
set(CMAKE_C_COMPILER_WORKS 1) #don't run compile test
set(CMAKE_CXX_COMPILER_WORKS 1)
find_program(CMAKE_C_COMPILER "${MW_BIN_PATH}/mwcceppc.exe") #Metrowerks C/C++ Compiler
find_program(CMAKE_CXX_COMPILER "${MW_BIN_PATH}/mwcceppc.exe")
find_program(CMAKE_LINKER "${MW_BIN_PATH}/mwldeppc.exe") #Metrowerks C/C++ Linker

#change compile rules so it behaves itself
set(CMAKE_C_COMPILE_OBJECT    "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>") #compile but don't link <file> -o <OBJECT>
set(CMAKE_C_LINK_EXECUTABLE   "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>") #<LINK_LIBRARIES>? use linker instead of compiler
set(CMAKE_CXX_COMPILE_OBJECT  "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(sdk_libs_str "\
${SDK_LIB_PATH}/aiD.a ${SDK_LIB_PATH}/amD.a \
${SDK_LIB_PATH}/amcnotstubD.a ${SDK_LIB_PATH}/amcstubsD.a \
${SDK_LIB_PATH}/arD.a ${SDK_LIB_PATH}/axD.a \
${SDK_LIB_PATH}/axartD.a ${SDK_LIB_PATH}/axfxD.a \
${SDK_LIB_PATH}/baseD.a ${SDK_LIB_PATH}/cardD.a \
${SDK_LIB_PATH}/dbD.a ${SDK_LIB_PATH}/demoD.a\
${SDK_LIB_PATH}/dspD.a ${SDK_LIB_PATH}/dtkD.a \
${SDK_LIB_PATH}/dvdD.a ${SDK_LIB_PATH}/exiD.a \
${SDK_LIB_PATH}/fileCacheD.a ${SDK_LIB_PATH}/G2DD.a \
${SDK_LIB_PATH}/gdD.a ${SDK_LIB_PATH}/gxD.a \
${SDK_LIB_PATH}/hioD.a ${SDK_LIB_PATH}/mccD.a \
${SDK_LIB_PATH}/mixD.a ${SDK_LIB_PATH}/mtxD.a \
${SDK_LIB_PATH}/odemustubsD.a ${SDK_LIB_PATH}/odenotstubD.a \
${SDK_LIB_PATH}/osD.a ${SDK_LIB_PATH}/padD.a \
${SDK_LIB_PATH}/perfD.a ${SDK_LIB_PATH}/seqD.a \
${SDK_LIB_PATH}/siD.a ${SDK_LIB_PATH}/spD.a \
${SDK_LIB_PATH}/supportD.a ${SDK_LIB_PATH}/synD.a \
${SDK_LIB_PATH}/texPaletteD.a ${SDK_LIB_PATH}/viD.a")
else()
	set(sdk_libs_str "\
${SDK_LIB_PATH}/ai.a ${SDK_LIB_PATH}/am.a \
${SDK_LIB_PATH}/amcnotstub.a ${SDK_LIB_PATH}/amcstubs.a \
${SDK_LIB_PATH}/ar.a ${SDK_LIB_PATH}/ax.a \
${SDK_LIB_PATH}/axart.a ${SDK_LIB_PATH}/axfx.a \
${SDK_LIB_PATH}/base.a ${SDK_LIB_PATH}/card.a \
${SDK_LIB_PATH}/db.a ${SDK_LIB_PATH}/demo.a \
${SDK_LIB_PATH}/dsp.a ${SDK_LIB_PATH}/dtk.a \
${SDK_LIB_PATH}/dvd.a ${SDK_LIB_PATH}/exi.a \
${SDK_LIB_PATH}/fileCache.a ${SDK_LIB_PATH}/G2D.a \
${SDK_LIB_PATH}/gd.a ${SDK_LIB_PATH}/gx.a \
${SDK_LIB_PATH}/hio.a ${SDK_LIB_PATH}/mcc.a \
${SDK_LIB_PATH}/mix.a ${SDK_LIB_PATH}/mtx.a \
${SDK_LIB_PATH}/odemustubs.a ${SDK_LIB_PATH}/odenotstub.a \
${SDK_LIB_PATH}/os.a ${SDK_LIB_PATH}/pad.a \
${SDK_LIB_PATH}/perf.a ${SDK_LIB_PATH}/seq.a \
${SDK_LIB_PATH}/si.a ${SDK_LIB_PATH}/sp.a \
${SDK_LIB_PATH}/support.a ${SDK_LIB_PATH}/syn.a \
${SDK_LIB_PATH}/texPalette.a ${SDK_LIB_PATH}/vi.a")
endif()

#set(MW_INC_PATH1 "${MW_BASE_PATH}/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include")
#set(MW_INC_PATH2 "${MW_BASE_PATH}/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include")
#set(ENV{MWCIncludes} "${MW_BASE_PATH}/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include:${MW_BASE_PATH}/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include")
#TODO: add more wrt C++?
set(mw_libs_str "\
${MW_LIB_PATH}/MSL/MSL_C/PPC_EABI/Lib/MSL_C.PPCEABI.bare.H.a \
${MW_LIB_PATH}/MetroTRK/TRK_Minnow_Dolphin.a \
${MW_LIB_PATH}/Runtime/Lib/Runtime.PPCEABI.H.a")
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(HW2_Prefix "${MW_LIB_PATH}/Include/Prefix/HW2_Debug_Prefix.h")
else()
	set(HW2_Prefix "${MW_LIB_PATH}/Include/Prefix/HW2_Release_Prefix.h")
endif()

set(musyx_libs_str "${MUSYX_LIB_PATH}/musyx.a")

set(CMAKE_CXX_FLAGS "-proc gekko -fp hard -maf on -enum int -ir ${SDK_INC_PATH} -ir ${MW_INC_PATH} -ir ${MUSYX_INC_PATH} -include ${HW2_Prefix}") #TODO: more?
set(CMAKE_CXX_FLAGS_DEBUG "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS "-gccinc -proc gekko -fp hard -maf on -enum int -ir ${SDK_INC_PATH} -ir ${MW_INC_PATH} -ir ${MUSYX_INC_PATH} -include ${HW2_Prefix}")
set(CMAKE_C_FLAGS_DEBUG "-Og")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_LINK_FLAGS "-l, ${sdk_libs_str} ${mw_libs_str} ${musyx_libs_str} -proc gekko -fp hard -nostdlib")
set(CMAKE_CXX_LINK_FLAGS "-l, ${sdk_libs_str} ${mw_libs_str} ${musyx_libs_str} -proc gekko -fp hard -nostdlib")