
SET(BUILD_COMPILER_armnoneeabigcc ON)
SET(PLATFORM_LPC1769              ON)

include (CMakeForceCompiler)
SET(CMAKE_SYSTEM_NAME GNU)

SET(CROSS_COMPILE_PREFIX  "arm-none-eabi-")

FIND_FILE(arm-gcc
    NAMES
        ${CROSS_COMPILE_PREFIX}gcc ${CROSS_COMPILE_PREFIX}gcc.exe
    PATHS
        $ENV{PATH}
)
FIND_FILE(arm-gxx
    NAMES
        ${CROSS_COMPILE_PREFIX}g++ ${CROSS_COMPILE_PREFIX}g++.exe
    PATHS
        $ENV{PATH}
)
FIND_FILE(arm-asm
    NAMES
        ${CROSS_COMPILE_PREFIX}as ${CROSS_COMPILE_PREFIX}as.exe
    PATHS
        $ENV{PATH}
)
FIND_FILE(arm-ar
    NAMES
        ${CROSS_COMPILE_PREFIX}ar ${CROSS_COMPILE_PREFIX}ar.exe
    PATHS
        $ENV{PATH}
)
FIND_FILE(arm-size
    NAMES
        ${CROSS_COMPILE_PREFIX}size ${CROSS_COMPILE_PREFIX}size.exe
    PATHS
        $ENV{PATH}
)

# specify the cross compiler
SET(CMAKE_C_COMPILER        "${arm-gcc}")
SET(CMAKE_CXX_COMPILER      "${arm-gxx}")

SET(CMAKE_AR                "${arm-ar}"  CACHE FILEPATH "Archiver")
SET(CMAKE_ASM_COMPILER      "${arm-asm}")
SET(CMAKE_LINKER            "${arm-gcc}")


SET(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -o <OBJECT> <SOURCE>")
#SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -rcs <TARGET> <LINK_FLAGS> <OBJECTS>")
#SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> -rcs <TARGET> <LINK_FLAGS> <OBJECTS>")

SET(CMAKE_ASM_COMPILER_ID   "GCC")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)

SET(CMAKE_EXECUTABLE_SUFFIX   ".elf")
SET(CMAKE_EXECUTABLE_SUFFIX_C ".elf")

CMAKE_FORCE_C_COMPILER("${CROSS_COMPILE_PREFIX}gcc"   "GNU")
CMAKE_FORCE_CXX_COMPILER("${CROSS_COMPILE_PREFIX}g++" "GNU")

IF(INCLUDING_TOOLCHAIN_FILE_FOR_FLAGS)

    ADD_DEFINITIONS("-msoft-float")
    ADD_DEFINITIONS("-MMD")
    ADD_DEFINITIONS("-MP")
    ADD_DEFINITIONS("-Wall")

    ADD_DEFINITIONS("-D__USE_LPCOPEN")
    ADD_DEFINITIONS("-DCORE_M3")
    ADD_DEFINITIONS("-D__REDLIB__")
    ADD_DEFINITIONS("-D__CODE_RED")
    #ADD_DEFINITIONS("-DDEBUG")
    ADD_DEFINITIONS("-Og")
    ADD_DEFINITIONS("-g3")
    ADD_DEFINITIONS("-Wall")
    ADD_DEFINITIONS("-c")
    ADD_DEFINITIONS("-fmessage-length=0")
    ADD_DEFINITIONS("-fno-builtin")
    ADD_DEFINITIONS("-ffunction-sections")
    ADD_DEFINITIONS("-fdata-sections")
    ADD_DEFINITIONS("-specs=redlib.specs")

    SET(DEBUG_FLAGS "-g3")

    SET(CORTEX_CPU_FLAG "-mcpu=cortex-m3 -mthumb -nostdlib")

    ADD_DEFINITIONS("${CORTEX_CPU_FLAG} ${DEBUG_FLAGS}")
ENDIF(INCLUDING_TOOLCHAIN_FILE_FOR_FLAGS)

IF(IS_DIRECTORY ${lpc_board_nxp_lpcxpresso_1769_SOURCE_DIR}/inc)
    include_directories(${lpc_board_nxp_lpcxpresso_1769_SOURCE_DIR}/inc)
ENDIF()
IF(IS_DIRECTORY ${lpc_chip_175x_6x_SOURCE_DIR}/inc)
    include_directories(${lpc_chip_175x_6x_SOURCE_DIR}/inc)
ENDIF()

SET(CMAKE_EXE_LINKER_FLAGS
    "${CORTEX_CPU_FLAG} ${DEBUG_FLAGS}"
)

IF(IS_DIRECTORY ${NxpRdLib_Platform_SOURCE_DIR}/src/Port/LPC1769)
    SET(LINKER_SCRIPT "${NxpRdLib_Platform_SOURCE_DIR}/src/Port/LPC1769/lpc1769.ld" )
    SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,--script ${LINKER_SCRIPT}"
    )
ENDIF()
