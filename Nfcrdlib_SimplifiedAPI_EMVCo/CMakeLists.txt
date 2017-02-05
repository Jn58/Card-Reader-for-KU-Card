

PROJECT(Nfcrdlib_SimplifiedAPI_EMVCo)


FILE(GLOB Nfcrdlib_SimplifiedAPI_EMVCo_Sources
    *.c
    src/*.c
)

include_directories(
    intfs
	types
)

ADD_EXECUTABLE(Nfcrdlib_SimplifiedAPI_EMVCo
    ${Nfcrdlib_SimplifiedAPI_EMVCo_Sources}
)

TARGET_LINK_LIBRARIES(
    Nfcrdlib_SimplifiedAPI_EMVCo
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
    NxpRdLib_phNfcLib
    NxpRdLib_palEpcUid
    curl
)

