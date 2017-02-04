

PROJECT(NxpRdLib_alI15693)

FILE(GLOB NxpRdLib_alI15693_Sources
    ./src/phalI15693.c
    ./src/Sw/phalI15693_Sw.c
    ./src/Sw/phalI15693_Sw.h
    ./src/Sw/phalI15693_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palSli15693
)
