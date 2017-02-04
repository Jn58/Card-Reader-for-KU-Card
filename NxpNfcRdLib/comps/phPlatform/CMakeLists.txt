

PROJECT(NxpRdLib_Platform)

FILE(GLOB NxpRdLib_Platform_Sources
    ./src/phPlatform.c
    ./src/Port/LPC11u68/cr_startup_lpc11u6x.c
    ./src/Port/LPC11u68/phPlatform_Port_LPC11u68.c
    ./src/Port/LPC1769/cr_startup_lpc175x_6x.c
    ./src/Port/LPC1769/phPlatform_Port_LPC1769.c
    ./src/Port/LPCTimer/phPlatform_Port_LPCTimer.c
    ./src/Port/PN7462AU/phPlatform_Port_PN7462AU.c
    ./src/Port/RaspberryPi/phPlatform_Port_Pi.c
    ./src/Port/RaspberryPi/phPlatform_Port_PiGpio.c
    ./src/Port/RaspberryPi/phPlatform_Port_PiGpio.h
)

IF(IS_DIRECTORY ${phhalTimer_SOURCE_DIR}/inc)
    INCLUDE_DIRECTORIES(${phhalTimer_SOURCE_DIR}/inc)
    INCLUDE_DIRECTORIES(${phBoot_SOURCE_DIR}/inc)
    INCLUDE_DIRECTORIES(${phhalPcr_SOURCE_DIR}/inc)
ENDIF()

ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_balReg
    NxpRdLib_Osal
    NxpRdLib_halHw
)

IF(BUILD_COMPILER_armnoneeabigcc AND PLATFORM_LPC1769)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        lpc_board_nxp_lpcxpresso_1769
        lpc_chip_175x_6x
        c
    )
ENDIF()
