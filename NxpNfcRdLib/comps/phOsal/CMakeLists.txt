

PROJECT(NxpRdLib_Osal)

FILE(GLOB NxpRdLib_Osal_Sources
    ./src/Abend/phOsal_Abend.c
    ./src/Freertos/phOsal_Freertos.c
    ./src/Nortos/phOsal_Nortos.c
    ./src/Winos/phOsal_Winos.c
    ./src/Winos/phOsal_Winos.h
    ./src/Winos/phOsal_Winos_Int.c
    ./src/NullOs/phOsal_NullOs.c
    ./src/Linux/phOsal_Linux.c
)
ADD_LIBRARY(NxpRdLib_Osal
    ${NxpRdLib_Osal_Sources}
)

IF(${PLATFORM_RaspberryPi})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        -lpthread
        -lrt
    )
ENDIF(${PLATFORM_RaspberryPi})

IF(${PLATFORM_RaspberryPi})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        -lpthread
        -lrt
    )
ENDIF(${PLATFORM_RaspberryPi})

IF(PLATFORM_LPC1769)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        LibOpenRTOS
    )
ENDIF(PLATFORM_LPC1769)
