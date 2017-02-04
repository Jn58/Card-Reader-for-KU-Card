

PROJECT(NfcrdlibEx6_EMVCo_Loopback)


FILE(GLOB NfcrdlibEx6_EMVCo_Loopback_Sources
    *.c
    src/*.c
)

include_directories(
    intfs
	types
)

ADD_EXECUTABLE(NfcrdlibEx6_EMVCo_Loopback
    ${NfcrdlibEx6_EMVCo_Loopback_Sources}
)

TARGET_LINK_LIBRARIES(
    NfcrdlibEx6_EMVCo_Loopback
    NxpRdLib_Osal
    NxpRdLib_Platform
    NxpRdLib_acDiscLoop
    NxpRdLib_halHw
    NxpRdLib_balReg
    NxpRdLib_KeyStore
    NxpRdLib_phTools
    NxpRdLib_palFelica
    NxpRdLib_palI18092mPI
    NxpRdLib_palI14443p4a
    NxpRdLib_palI14443p4
    NxpRdLib_palI14443p4
    NxpRdLib_palMifare
    NxpRdLib_alFelica
    NxpRdLib_alMful
    NxpRdLib_alMfdf
    NxpRdLib_alT1T
    NxpRdLib_alTop
    NxpRdLib_alMfc
    NxpRdLib_lnLlcp
    NxpRdLib_phTools
    NxpRdLib_palI18092mT
    NxpRdLib_npSnep
    NxpRdLib_palI14443p4mC
    NxpRdLib_ceT4T
    NxpRdLib_palEpcUid
)

