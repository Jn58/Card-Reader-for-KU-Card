

PROJECT(NxpRdLib_alSli)

FILE(GLOB NxpRdLib_alSli_Sources
    ./src/phalSli.c
    ./src/phalSli_Int.h
    ./src/Sw/phalSli_Sw.c
    ./src/Sw/phalSli_Sw.h
    ./src/Sw/phalSli_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)
