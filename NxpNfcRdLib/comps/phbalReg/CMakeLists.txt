

PROJECT(NxpRdLib_balReg)

FILE(GLOB NxpRdLib_balReg_Sources
    ./src/phbalReg.c
    ./src/LpcOpenI2C/phbalReg_LpcOpenI2c.c
    ./src/LpcOpenI2C/phbalReg_LpcOpenI2c.h
    ./src/LpcOpenSPI/phbalReg_LpcOpenSpi.c
    ./src/LpcOpenSPI/phbalReg_LpcOpenSpi.h
    ./src/LinuxUserSpi/phbalReg_LinuxUserSpi.c
    ./src/LinuxUserSpi/phbalReg_LinuxUserSpi.h
    ./src/LinuxKernelSpi/phbalReg_LinuxKernelSpi.c
    ./src/LinuxKernelSpi/phbalReg_LinuxKernelSpi.h
    ./src/LinuxUserI2C/phbalReg_LinuxUserI2C.c
    ./src/LinuxUserI2C/phbalReg_LinuxUserI2C.h
)
ADD_LIBRARY(NxpRdLib_balReg
    ${NxpRdLib_balReg_Sources}
)


IF(BUILD_COMPILER_armnoneeabigcc AND PLATFORM_LPC1769)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        lpc_board_nxp_lpcxpresso_1769
        lpc_chip_175x_6x
        c
    )
ENDIF()
