/*
 * phNfcLib_Int.h
 *
 *  Created on: May 20, 2016
 *      Author: nxp69453
 */

#ifndef PHNFCLIB_INT_H
#define PHNFCLIB_INT_H

#include <phNfcLib.h>

typedef phStatus_t (*pphNfcLib_Config_Profile)();
typedef phStatus_t (*pphNfcLib_Activate_Profile)(uint16_t wTechnologyMask);
typedef phStatus_t (*pphNfcLib_Deactivate_Profile)();
typedef phStatus_t (*pphNfcLib_Transmit)(void * const  pTxBuffer, uint16_t wTxBufferLength);


#define PH_NFCLIB_INT_TRANSMIT_OFF                  0x00U
#define PH_NFCLIB_INT_TRANSMIT_ON                   0x01U
#define PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING     0x02U
/**
 * \brief NFC LIB State
 * \anchor NfcLib_State
 */
typedef struct
{
    uint8_t  bNfcLibState;      /* State of the NfcLib takes value from enum phNfcLib_StateMachine_t */
    uint8_t  bProfileSelected;  /* Tells about the selected profile takes value ISO, NFC, EMVCO */
    uint8_t  bActivateBlocking; /* Tells the blocking #PH_ON or #PH_OFF feature of Activation */
    uint8_t  bDeactBlocking;    /* Tells the blocking #PH_ON or #PH_OFF feature of DeActivation */
    uint8_t  bMergedSakPrio;    /* Tells the priority to perform activation of the card in case Merged Sak is received */
    uint8_t  bLPCDState;        /* Tells the LPCD #PH_ON or #PH_OFF performance */
    uint8_t  bTxState;          /* Tells the TX state of nfclib, if 1 then only receive can be performed */
    uint16_t wConfiguredRFTech; /* Tells the configured RF Tech that was used for current activation cycle */
    phNfcLib_PeerInfo_t * pPeerInfo;
    uint8_t  *pRxBuffer;        /* Pointer to the RX */
    uint16_t wRxLength;         /* Stores the length of the received Data */
    uint16_t  wRxDataPoin;       /* Points to the starting address of the rx buffer to perform Chaining */
    uint8_t  bHalBufferTx[PH_NFCLIB_HAL_TX_BUFFSIZE];          /* HAL TX buffer. */
    uint8_t  bHalBufferRx[PH_NFCLIB_HAL_RX_BUFFSIZE];          /* HAL RX buffer. */
    uint8_t bWrappedMode;     /* Tells about the Mifare Desfire Wrapped Mode Status */
    uint8_t bAuthMode;        /* Tells about the Mifare Desfire Auth Mode */
    uint16_t wActivatedUIDLength; /* Length of the activated UID */
    uint8_t* pActivatedUid;  /* Contains the pointer of the activated Uid */

} phNfcLib_InternalState_t;

extern phNfcLib_DataParams_t    gphNfcLib_Params;
extern phNfcLib_InternalState_t gphNfcLib_State;

/*
 * ATQ codes
 */
#define PH_NFCLIB_ATQA_UL        0x4400
#define PH_NFCLIB_ATQA_ULC       0x4400
#define PH_NFCLIB_ATQA_MFC       0x0200
#define PH_NFCLIB_ATQA_MFP_S     0x0400
#define PH_NFCLIB_ATQA_MFP_X     0x4200
#define PH_NFCLIB_ATQA_DESFIRE   0x4403
#define PH_NFCLIB_ATQA_DESFIRE1   0x0403
#define PH_NFCLIB_ATQA_JCOP      0x0400
#define PH_NFCLIB_ATQA_MINI      0x0400
#define PH_NFCLIB_ATQA_NPA       0x0800

/*
 * SAK codes
 */
#define PH_NFCLIB_SAK_UL         0x00
#define PH_NFCLIB_SAK_ULC        0x00 //ATQA and SAK for UL and ULC are same.
#define PH_NFCLIB_SAK_MINI       0x09
#define PH_NFCLIB_SAK_MFC_1K     0x08
#define PH_NFCLIB_SAK_MFC_4K     0x18
#define PH_NFCLIB_SAK_MFP_2K_SL1 0x08
#define PH_NFCLIB_SAK_MFP_4K_SL1 0x18
#define PH_NFCLIB_SAK_MFP_2K_SL2 0x10
#define PH_NFCLIB_SAK_MFP_4K_SL2 0x11
#define PH_NFCLIB_SAK_MFP_2K_SL3 0x20
#define PH_NFCLIB_SAK_MFP_4K_SL3 0x20
#define PH_NFCLIB_SAK_DESFIRE    0x20 //0x24
#define PH_NFCLIB_SAK_JCOP       0x28
#define PH_NFCLIB_SAK_LAYER4     0x20
#define PH_NFCLIB_SAK_P2P        0x40
#define PH_NFCLIB_SAK_MERGED     0x60

/* By default invalid authenticatation status */
#define PH_NFCLIB_MFDF_NOT_AUTHENTICATED             0xFFU   /**< No authentication. */

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFC
phStatus_t phNfcLib_MFC_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFC */

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFUL
phStatus_t phNfcLib_MFUL_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFUL */

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFDF
phStatus_t phNfcLib_MFDF_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFDF */

#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
phStatus_t phNfcLib_ISO15693_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693 */

#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
phStatus_t phNfcLib_ISO18000_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000*/

#if (defined NXPBUILD__PH_NFCLIB_ISO_MFC || defined NXPBUILD__PH_NFCLIB_ISO_MFUL)
/* Utility API */
phStatus_t phNfcLib_Mifare_Transmit(uint8_t * pTxBuffer, uint16_t wTxLength);
#endif

#endif /* PHNFCLIB_INT_H */
