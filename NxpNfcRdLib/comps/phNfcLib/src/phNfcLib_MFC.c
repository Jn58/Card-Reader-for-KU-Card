/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NFC Library Top Level API of Reader Library Framework.
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v4.040.05.011646)
* $Date: 2016-05-20 18:04:59 +0530 (Fri, 20 May 2016) $
*
* History:
*  KK: Generated 20. May 2016
*/


#include <ph_Status.h>
#include <phNfcLib.h>
#include "phNfcLib_Int.h"
#include <phKeyStore.h>

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFC
/**
 * Macro for the Mifare Classic Commands
 */
#define PH_NFCLIB_MFC_READ        0x30    /**< MIFARE Classic Read command byte */
#define PH_NFCLIB_MFC_WRITE       0xA0    /**< MIFARE Classic Write command byte */
#define PH_NFCLIB_MFC_INCREMENT   0xC1    /**< MIFARE Classic Increment command byte */
#define PH_NFCLIB_MFC_DECREMENT   0xC0    /**< MIFARE Classic Decrement command byte */
#define PH_NFCLIB_MFC_RESTORE     0xC2    /**< MIFARE Classic Restore command byte */
#define PH_NFCLIB_MFC_TRANSFER    0xB0    /**< MIFARE Classic Transfer command byte */
#define PH_NFCLIB_MFC_PERSOUID    0x40    /**< MIFARE Classic Personalize UID command */


phStatus_t phNfcLib_MFC_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t wStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t    aCommand[2] = {0};
    uint8_t    aKey[PHHAL_HW_MFC_KEY_LENGTH * 2];
#ifdef NXPBUILD__PH_KEYSTORE_SW
    uint16_t   bKeystoreKeyType;
#endif /* NXPBUILD__PH_KEYSTORE_SW */

    switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bCommand)
    {
    case Read:
        aCommand[0] = PH_NFCLIB_MFC_READ;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;

        wStatus = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            aCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
        break;

    case Write:
        aCommand[0] = PH_NFCLIB_MFC_WRITE;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;

        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            2));

        wStatus = phNfcLib_Mifare_Transmit(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.pBuffer,
            16);
        break;

    case Increment:
        aCommand[0] = PH_NFCLIB_MFC_INCREMENT;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;


        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            2));

        wStatus = phNfcLib_Mifare_Transmit(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.pBuffer,
            4);
        if((wStatus & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            return PH_ERR_SUCCESS;
        }
        break;

    case Decrement:
        aCommand[0] = PH_NFCLIB_MFC_DECREMENT;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;

        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            2));

        wStatus = phNfcLib_Mifare_Transmit(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.pBuffer,
            4);
        if((wStatus & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            return PH_ERR_SUCCESS;
        }
        break;

    case Restore:
        memset(&gphNfcLib_State.bHalBufferRx[0], 0x00, 4);  /* PRQA S 3200 */
        aCommand[0] = PH_NFCLIB_MFC_RESTORE;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;

        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            2));

        wStatus = phNfcLib_Mifare_Transmit(
            &gphNfcLib_State.bHalBufferRx[0],
            4);
        if((wStatus & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            return PH_ERR_SUCCESS;
        }
        break;

    case Transfer:
        aCommand[0] = PH_NFCLIB_MFC_TRANSFER;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber;

        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            2));
        break;

    case Authenticate:
#ifdef  NXPBUILD__PH_KEYSTORE_SW
        PH_CHECK_SUCCESS_FCT(wStatus, phKeyStore_GetKey(
            &gphNfcLib_Params.sPlatform.sKeyStore,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bKeyNumber,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bKeyVersion,
            sizeof(aKey),
            &aKey[0],
            &bKeystoreKeyType));
#endif /* NXPBUILD__PH_KEYSTORE_SW */

        wStatus = phhalHw_MfcAuthenticate(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bBlockNumber,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bKeyType,
            &aKey[0],
            (gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso14443_3a.bUidLength == 4) ? (&gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso14443_3a.aUid[0]) : (&gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso14443_3a.aUid[3])
            );
        break;

    case PersonalizeUID:
        aCommand[0] = PH_NFCLIB_MFC_PERSOUID;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareClassic.bUidType;

        wStatus = phNfcLib_Mifare_Transmit(
            aCommand,
            2);
        break;

    default:
        break;
    }

    return wStatus;
}

#endif /* NXPBUILD__PH_NFCLIB_ISO_MFC*/
