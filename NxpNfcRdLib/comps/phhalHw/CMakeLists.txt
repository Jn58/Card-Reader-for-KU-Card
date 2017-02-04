

PROJECT(NxpRdLib_halHw)

FILE(GLOB ${PROJECT_NAME}_Sources
    ./src/phhalHw.c
    ./src/Pn5180/phhalHw_Pn5180.c
    ./src/Pn5180/phhalHw_Pn5180.h
    ./src/Pn5180/phhalHw_Pn5180_Instr.c
    ./src/Pn5180/phhalHw_Pn5180_Int.c
    ./src/Pn5180/phhalHw_Pn5180_Int.h
    ./src/Pn5180/phhalHw_Pn5180_Wait.c
    ./src/Pn5180/phhalHw_Pn5180_Wait.h
    ./src/PN7462AU/phhalHw_PN7462AU.c
    ./src/PN7462AU/phhalHw_PN7462AU.h
    ./src/PN7462AU/phhalHw_PN7462AU_Int.c
    ./src/PN7462AU/phhalHw_PN7462AU_Int.h
    ./src/Rc523/phhalHw_Rc523.c
    ./src/Rc523/phhalHw_Rc523.h
    ./src/Rc523/phhalHw_Rc523_Cmd.c
    ./src/Rc523/phhalHw_Rc523_Config.h
    ./src/Rc523/phhalHw_Rc523_Int.c
    ./src/Rc523/phhalHw_Rc523_Int.h
    ./src/Rc523/phhalHw_Rc523_Wait.c
    ./src/Rc663/phhalHw_Rc663.c
    ./src/Rc663/phhalHw_Rc663.h
    ./src/Rc663/phhalHw_Rc663_Cmd.c
    ./src/Rc663/phhalHw_Rc663_Config.h
    ./src/Rc663/phhalHw_Rc663_Int.c
    ./src/Rc663/phhalHw_Rc663_Int.h
    ./src/Rc663/phhalHw_Rc663_Wait.c
)

ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)


TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_phTools
    NxpRdLib_Osal
    NxpRdLib_KeyStore
    NxpRdLib_balReg
    NxpRdLib_Platform
)


IF(PLATFORM_LPC1769)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        NxpRdLib_phTools
        NxpRdLib_Osal
        NxpRdLib_KeyStore
        NxpRdLib_balReg
    )
ENDIF(PLATFORM_LPC1769)