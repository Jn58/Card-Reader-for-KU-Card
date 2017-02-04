
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

ADD_SUBDIRECTORY(NfcrdlibEx1_BasicDiscoveryLoop)
ADD_SUBDIRECTORY(NfcrdlibEx2_AdvancedDiscoveryLoop)
ADD_SUBDIRECTORY(NfcrdlibEx3_NFCForum)
ADD_SUBDIRECTORY(NfcrdlibEx4_MIFAREClassic)
ADD_SUBDIRECTORY(NfcrdlibEx5_ISO15693)
ADD_SUBDIRECTORY(NfcrdlibEx6_EMVCo_Loopback)
ADD_SUBDIRECTORY(NfcrdlibEx7_EMVCo_Polling)
ADD_SUBDIRECTORY(NfcrdlibEx8_HCE_T4T)
ADD_SUBDIRECTORY(NfcrdlibEx9_NTagI2C)
ADD_SUBDIRECTORY(Nfcrdlib_SimplifiedAPI_EMVCo)
ADD_SUBDIRECTORY(Nfcrdlib_SimplifiedAPI_ISO)
