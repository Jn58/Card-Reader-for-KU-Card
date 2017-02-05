
cmake_minimum_required(VERSION 2.8)

OPTION(PLATFORM_RaspberryPi  "Platform is RaspberryPi"                                ON)

PROJECT(NxpNfcRdLibSW)

IF(${PLATFORM_RaspberryPi})
    ADD_DEFINITIONS(-pthread)
    ADD_DEFINITIONS(-DDEBUG)
    ADD_DEFINITIONS(-DNXPBUILD__PH_RASPBERRY_PI)
ENDIF(${PLATFORM_RaspberryPi})

INCLUDE_DIRECTORIES(
    NxpNfcRdLib/intfs
    NxpNfcRdLib/types
)

ADD_SUBDIRECTORY(NxpNfcRdLib)

ADD_SUBDIRECTORY(Nfcrdlib_SimplifiedAPI_EMVCo)
