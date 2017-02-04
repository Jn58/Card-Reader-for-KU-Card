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

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFDF

#define PH_NFCLIB_MFDF_CREATE_APPLN                  0xCA    /**< MF DF Create Application cmd. */
#define PH_NFCLIB_MFDF_CMD_SELECT_APPLN              0x5A    /**< MF DF Select Application Cmd. */
#define PH_NFCLIB_MFDF_CMD_GET_VERSION               0x60    /**< MF DF Get Version cmd. */
#define PH_NFCLIB_MFDF_CMD_CREATE_STD_DATAFILE       0xCD    /**< MF DF Create Standard data file cmd. */
#define PH_NFCLIB_MFDF_CMD_WRITE_DATA                0x3D    /**< MF DF Write data cmd. */

#define PH_NFCLIB_MFDF_COMMUNICATION_PLAIN           0x00U   /**< Plain mode of communication. */

#define PH_NFCLIB_MFDF_CMD_ISO7816_SELECT_FILE       0xA4    /**< ISO Select File. */
#define PH_NFCLIB_MFDF_CMD_ISO7816_READ_BINARY       0xB0    /**< ISO Read Binary. */
#define PH_NFCLIB_MFDF_CMD_ISO7816_UPDATE_BINARY     0xD6    /**< ISO UPDATE Binary. */

#define PH_NFCLIB_MFDF_WRAP_HDR_LEN                  0x05U   /* Wrapped APDU header length */
#define PH_NFCLIB_MFDF_WRAPPEDAPDU_CLA               0x90U   /* Wrapped APDU default class. */
#define PH_NFCLIB_MFDF_WRAPPEDAPDU_P1                0x00U   /* Wrapped APDU default P1. */
#define PH_NFCLIB_MFDF_WRAPPEDAPDU_P2                0x00U   /* Wrapped APDU default P2. */
#define PH_NFCLIB_MFDF_WRAPPEDAPDU_LE                0x00U   /* Wrapped APDU default LE. */
#define PH_NFCLIB_MFDF_MAXWRAPPEDAPDU_SIZE           0x37u   /**< Maximum size of wrapped APDU 55 bytes. */
#define PH_NFCLIB_MFDF_MAXDFAPDU_SIZE                0x3Cu   /**< Maximum size of DESFire APDU 60 bytes. */
#define PH_NFCLIB_MFDF_DFAPPID_SIZE                  0x03u   /**< Size of MF DF application Id. */
#define PH_NFCLIB_MFDF_MAX_FRAME_SIZE                0x40u   /**< Max size in a ISO 14443-4 frame. */
#define PH_NFCLIB_MFDF_RESP_OPERATION_OK             0x00U    /**< MF DF Response - Successful operation. */
#define PH_NFCLIB_MFDF_ISO7816_SUCCESS               0x90U /**< Correct execution. */
#define PH_NFCLIB_MFDF_ISO7816_SUCCESS_STATUS        0x9000U /**< Correct execution. */
#define PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME         0xAFU    /**< MF DF Additional data frame is expected to be sent. */


static phStatus_t phNfcLib_MFDF_Write_Plain(uint8_t * bCmdBuff, uint16_t wCmdLen, uint8_t bCommOption, uint8_t * pData, uint16_t  wDataLen);
static phStatus_t phNfcLib_MFDF_Exchange_Cmd(uint8_t bWrappedMode,  uint8_t * pCmd, uint16_t wCmdLen,  uint8_t ** ppResponse, uint16_t * wRxlen);
static phStatus_t phNfcLib_MFDF_SendDataToPICC(uint8_t * pCmd, uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * pResp, uint16_t * pRespLen );
static phStatus_t phNfcLib_MFDF_GetData(
                          uint8_t * pSendBuff,
                          uint16_t wCmdLen,
                          uint8_t ** pResponse,
                          uint16_t * pRxlen
                          );
static phStatus_t phNfcLib_MFDF_ReadData_Plain(
                                        uint8_t bOption,
                                        uint8_t * bCmdBuff,
                                        uint16_t wCmdLen,
                                        uint8_t ** ppRxdata,
                                        uint16_t * pRxdataLen
                                        );

static phStatus_t phNfcLib_MFDF_Send7816Apdu(
                                    uint8_t bOption,
                                    uint8_t bIns,
                                    uint8_t bP1,
                                    uint8_t bP2,
                                    uint8_t bLc,
                                    uint8_t * pDataIn,
                                    uint8_t bLe,
                                    uint8_t ** ppDataOut,
                                    uint16_t *pDataLen
                                    );

static phStatus_t phNfcLib_MFDF_IsoRead(
                               uint16_t wOption,
                               uint8_t * bCmdBuff,
                               uint16_t wCmdLen,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pBytesRead
                               );


phStatus_t phNfcLib_MFDF_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t wStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t aCommand[32] = {0};
    uint16_t wCmdLen = 0;
    uint16_t  wDataLen;
    uint8_t bLc = 0;
    uint8_t      bP1 = 0;
    uint8_t      bP2 = 0;
    uint8_t      bLe = 0;
    uint8_t      bCmdBuff[8];
    uint16_t     wOffset;

    switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.bCommand)
    {

    case MFDF_Create_Application:

        /*
        If (bKeySettings2 & 0x03)== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
        0x0A or 0x1A.
        If (bKeySettings2 & 0x03)== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
        If (bKeySettings2 & 0x03)== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
        */

        /* Checking the input paramteter */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bISODFNameLen > 16) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bOptions > 0x03) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bOptions == 0x02))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }

        /* form the command */
        aCommand[wCmdLen++] = PH_NFCLIB_MFDF_CREATE_APPLN;

        memcpy(&aCommand[wCmdLen], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.pApplicationIdentifier[0], 3); /* PRQA S 3200 */
        wCmdLen += 3;

        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bKeySettings1;
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bKeySettings2;

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bOptions & 0x01)
        {
            /* wISOFileId is present */
            aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.pISOFileId[0];
            aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.pISOFileId[1];
        }
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bOptions & 0x02)
        {
            /* pISODFName is present */
            memcpy(&aCommand[wCmdLen], ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.pISODFName, ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bISODFNameLen); /* PRQA S 3200 */
            wCmdLen = wCmdLen + ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_Application.bISODFNameLen;
        }

        return  phNfcLib_MFDF_Write_Plain(
            aCommand,
            wCmdLen,
            PH_NFCLIB_MFDF_COMMUNICATION_PLAIN,
            NULL,
            0x0000
            );


        break;

    case MFDF_Select_Application:

        /* form the command */
        aCommand[0] = PH_NFCLIB_MFDF_CMD_SELECT_APPLN;
        memcpy(&aCommand[1], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_Application.pApplicationIdentifier[0], PH_NFCLIB_MFDF_DFAPPID_SIZE); /* PRQA S 3200 */

        /* Send the command */
        return phNfcLib_MFDF_Exchange_Cmd(
            gphNfcLib_State.bWrappedMode,
            &aCommand[0],
            4,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);


        break;

    case MFDF_Get_Version:

        /* form the command */
        aCommand[0] = PH_NFCLIB_MFDF_CMD_GET_VERSION;

        wStatus = phNfcLib_MFDF_ReadData_Plain(
            PH_NFCLIB_MFDF_COMMUNICATION_PLAIN,
            &aCommand[0],
            1,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);


        if (gphNfcLib_State.wRxLength != 28)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        break;

    case MFDF_Create_StdDataFile:

        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bFileNo > 0x1f) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bOption > 0x01))
        {
            return PH_ERR_INVALID_PARAMETER;
        }


        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bCommSett != PH_NFCLIB_MFDF_COMMUNICATION_PLAIN)
        {
            return PH_ERR_INVALID_PARAMETER;
        }


        /* form the command */
        aCommand[wCmdLen++] = PH_NFCLIB_MFDF_CMD_CREATE_STD_DATAFILE;

        /* File Number */
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bFileNo;

        /* Copy ISO Filed ID if present */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bOption == 0x01)
        {
            aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pISOFileId[0];
            aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pISOFileId[1];
        }

        /* Copy communication settings. communication settings in the first nibble so right shifting */
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bCommSett >> 4;

        /* Copy Access rights */
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pAccessRights[0];
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pAccessRights[1];

        /* Copy File size supplied by the user */
        memcpy(&aCommand[wCmdLen], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pFileSize[0], 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* COMMUNICATION IS PLAIN */
        return phNfcLib_MFDF_Write_Plain(
            aCommand,
            wCmdLen,
            PH_NFCLIB_MFDF_COMMUNICATION_PLAIN,
            NULL,
            0x0000
            );

        break;

    case MFDF_Write_Data:

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.bFileNo > 0x1f)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.bCommOption != PH_NFCLIB_MFDF_COMMUNICATION_PLAIN)
        {
            return PH_ERR_UNSUPPORTED_PARAMETER;
        }

        /* form the command */
        aCommand[wCmdLen++] = PH_NFCLIB_MFDF_CMD_WRITE_DATA;
        aCommand[wCmdLen++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.bFileNo;
        memcpy(&aCommand[wCmdLen], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.pOffset[0], 3); /* PRQA S 3200 */
        wCmdLen += 3;
        memcpy(&aCommand[wCmdLen], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.pTxDataLen[0], 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* Assuming here that the size can never go beyond FFFF.
        In fact it can never go beyond 8092 (1F9C) bytes */
        wDataLen = (uint16_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.pTxDataLen[1];
        wDataLen = wDataLen << 8;
        wDataLen |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.pTxDataLen[0];

        return phNfcLib_MFDF_Write_Plain(
            aCommand,
            wCmdLen,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.bCommOption,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Write_Data.pTxData,
            wDataLen
            );

        break;

    case MFDF_Iso_SelectFile:

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bDFnameLen > 16)
        {
            return PH_ERR_INVALID_PARAMETER;
        }
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bOption != 0x00) && (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bOption != 0x0C))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bSelector)
        {
        case 0x00:
        case 0x02:
            /* Selection by EF Id*/
            /* Send MSB first to card */
            aCommand[0] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.pFid[1];
            aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.pFid[0];
            bLc = 2;
            break;

        case 0x04:
            /* Selection by DF Name */
            memcpy(aCommand, ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.pDFname, ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bDFnameLen); /* PRQA S 3200 */
            bLc = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bDFnameLen;
            break;

        default:
            return PH_ERR_INVALID_PARAMETER;
        }
        return  phNfcLib_MFDF_Send7816Apdu(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bOption == 0x0C ? 0x01 : 0x03,             /* As per Table 40-P2 in ISO/IEC FDIS 7816-4 */
            PH_NFCLIB_MFDF_CMD_ISO7816_SELECT_FILE,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bSelector,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Select_File.bOption,
            bLc,
            aCommand,
            0x00,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* ISO wrapped mode is on */
        gphNfcLib_State.bWrappedMode = 1;

        return PH_ERR_SUCCESS;
        break;

    case MFDF_Iso_ReadBinary:

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.wOption == PH_EXCHANGE_DEFAULT)
        {
            if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bSfid & 0x80)
            {
                /* Short file id is supplied */
                if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bSfid & 0x7FU) > 0x1F)
                {
                    /* Error condition */
                    return PH_ERR_INVALID_PARAMETER;
                }
                bP1 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bSfid;
                bP2 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bOffset;
            }
            else
            {
                /* P1 and P2 code the offset */
                wOffset = bP1 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bSfid;;
                wOffset <<= 8; /* Left shift */
                wOffset |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bOffset;;
                bP2 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bOffset;;
            }
            bLe = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.bBytesToRead;
            bCmdBuff[wCmdLen++] = 0x00; /* Class */
            bCmdBuff[wCmdLen++] = PH_NFCLIB_MFDF_CMD_ISO7816_READ_BINARY; /* Ins */
            bCmdBuff[wCmdLen++] = bP1;
            bCmdBuff[wCmdLen++] = bP2;
            bCmdBuff[wCmdLen++] = bLe; /* Le */
        }
        else if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.wOption == PH_EXCHANGE_RXCHAINING)
        {
            wCmdLen = 0;
        }
        else
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        return phNfcLib_MFDF_IsoRead(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Read_Binary.wOption,
            bCmdBuff,
            wCmdLen,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
        break;

    case MFDF_Iso_UpdateBinary:

        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bSfid & 0x80)
        {
            /* Short file id is supplied */
            if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bSfid & 0x7FU) > 0x1F)
            {
                /* Error condition */
                return PH_ERR_INVALID_PARAMETER;
            }
            bP2 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bOffset;
        }
        else
        {
            /* P1 and P2 code the offset */
            wOffset = bP1 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bSfid;
            wOffset <<= 8; /* Left shift */
            wOffset |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bOffset;
            bP2 = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bOffset;
        }

        bLc = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.bDataLen;

        wStatus =  phNfcLib_MFDF_Send7816Apdu(
            0x01,
            PH_NFCLIB_MFDF_CMD_ISO7816_UPDATE_BINARY,
            bP1,
            bP2,
            bLc,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareDesfire.Operation.Update_Binary.pData,
            0x00,
            NULL,
            NULL);

        break;
    default:
        break;
    }

    return wStatus;
}


/*
 * To perform the write plain option of MIfare Desfire, contains bCommOption extra parameter for crypto integration
 */
static phStatus_t phNfcLib_MFDF_Write_Plain(uint8_t * bCmdBuff, uint16_t wCmdLen, uint8_t bCommOption, uint8_t * pData, uint16_t  wDataLen)
{
    phStatus_t   statusTmp = 0;
    uint8_t      bWorkBuffer[32];
    uint16_t     wFrameLen = 0;
    uint8_t      pApdu[5] = { PH_NFCLIB_MFDF_WRAPPEDAPDU_CLA, 0x00, PH_NFCLIB_MFDF_WRAPPEDAPDU_P1, PH_NFCLIB_MFDF_WRAPPEDAPDU_P2, 0x00 };

    memset(bWorkBuffer, 0x00, 16); /* PRQA S 3200 */

    if (gphNfcLib_State.bWrappedMode)
    {
        wFrameLen = PH_NFCLIB_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PH_NFCLIB_MFDF_MAXDFAPDU_SIZE;
    }

    if (wDataLen == 0x0000)
    {
        /* Single frame cmd without any data. Just send it */
        statusTmp = phNfcLib_MFDF_Exchange_Cmd(
            gphNfcLib_State.bWrappedMode,
            bCmdBuff,
            wCmdLen,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        if (statusTmp != PH_ERR_SUCCESS)
        {
            return statusTmp;
        }

        if (gphNfcLib_State.wRxLength > 32)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
    }
    else
    {
        if (wDataLen <= (wFrameLen - wCmdLen))
        {
            /* Send in one shot */
            if (gphNfcLib_State.bWrappedMode)
            {
                pApdu[1] = bCmdBuff[0]; /* Desfire cmd code in INS */
                pApdu[4] = (uint8_t)(wCmdLen + wDataLen) - 0x01u;

                /**
                * This will overwrite the consecutive ram memory if the halBufferRX is kept ridiculously low
                */

                /* There could be two approaches here */
                /** Buffer last can be kept seperately to accomodate  the crypto */
                /** The other approach can be to perform altogether different exchanges for crypto and non crypto mode */

                memcpy(&gphNfcLib_State.bHalBufferRx[0], &pApdu[0], 5);    /* PRQA S 3200 */
                memcpy(&gphNfcLib_State.bHalBufferRx[5], &bCmdBuff[1], wCmdLen - 1);    /* PRQA S 3200 */
                memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen], pData, wDataLen);    /* PRQA S 3200 */
                memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen + wDataLen], &pApdu[2], 0x01);    /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_DEFAULT,
                    &gphNfcLib_State.bHalBufferRx[0],
                    5 + wCmdLen + wDataLen,
                    &gphNfcLib_State.pRxBuffer,
                    &gphNfcLib_State.wRxLength));

/** In case the above code does not work */
#if 0
                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_FIRST,
                    pApdu,
                    PH_NFCLIB_MFDF_WRAP_HDR_LEN,
                    &pRecv,
                    &wRxlen));

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_CONT,
                    &bCmdBuff[1],
                    wCmdLen - 1,
                    &pRecv,
                    &wRxlen));


                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_CONT,
                    pData,
                    wDataLen,
                    &pRecv,
                    &wRxlen));


                /* Le Byte */
                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_LAST,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    &wRxlen));

                /*Uncheck the above piece of code and use this     */
#endif
            }
            else
            {
                memcpy(&gphNfcLib_State.bHalBufferRx[0], &bCmdBuff[0], wCmdLen);    /* PRQA S 3200 */
                memcpy(&gphNfcLib_State.bHalBufferRx[wCmdLen], pData, wDataLen);    /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_DEFAULT,
                    &gphNfcLib_State.bHalBufferRx[0],
                    wCmdLen + wDataLen,
                    &gphNfcLib_State.pRxBuffer,
                    &gphNfcLib_State.wRxLength));


#if 0
                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_FIRST,
                    bCmdBuff,
                    wCmdLen,
                    &pRecv,
                    &wRxlen));

                /** Buffer last is kept seperately to accomodate  the crypto */
                /** The other approach can be to perform altogether different exchanges for crypto and non crypto mode */

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_LAST,
                    pData,
                    wDataLen,
                    &pRecv,
                    &wRxlen));

#endif

            }
            if (gphNfcLib_State.bWrappedMode)
            {
                statusTmp = gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 1];
                gphNfcLib_State.wRxLength -= 2;
            }
            else
            {
                statusTmp = gphNfcLib_State.pRxBuffer[0];
                gphNfcLib_State.pRxBuffer++; /* Increment pointer to point only to data */
                gphNfcLib_State.wRxLength -= 1;
            }

            if (statusTmp != PH_ERR_SUCCESS)
            {

                if(statusTmp != PH_NFCLIB_MFDF_ISO7816_SUCCESS)
                {
                    (statusTmp == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME) ? (statusTmp =  PH_ERR_SUCCESS_CHAINING) : (statusTmp = PH_ERR_PROTOCOL_ERROR);
                }
                else
                {
                    statusTmp = PH_ERR_SUCCESS;
                }
                return statusTmp;

            }

            memcpy(bWorkBuffer, gphNfcLib_State.pRxBuffer, gphNfcLib_State.wRxLength); /* PRQA S 3200 */
        }
        else
        {
            /* Send command and data. Chain data to PICC */
            statusTmp = phNfcLib_MFDF_SendDataToPICC(
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen,
                bWorkBuffer,
                &gphNfcLib_State.wRxLength
                );

        }
    }

    /* Should not get more bytes than the status bytes in case
    of no authentication */
    if (gphNfcLib_State.wRxLength > 0)
    {
        return PH_ERR_PROTOCOL_ERROR;
    }

    return PH_ERR_SUCCESS;

}


/*
 * This api performs the mifare desfire exchange
 */
static phStatus_t phNfcLib_MFDF_Exchange_Cmd(uint8_t bWrappedMode,  uint8_t * pSendBuff, uint16_t wCmdLen,  uint8_t ** ppResponse, uint16_t * pRxlen)
{
    uint8_t      * pRecv;
    phStatus_t   statusTmp;
    uint8_t      pApdu[5] = { PH_NFCLIB_MFDF_WRAPPEDAPDU_CLA, 0x00, PH_NFCLIB_MFDF_WRAPPEDAPDU_P1, PH_NFCLIB_MFDF_WRAPPEDAPDU_P2, 0x00 };


    if(((bWrappedMode) && (wCmdLen > PH_NFCLIB_MFDF_MAXWRAPPEDAPDU_SIZE))
        || ((!bWrappedMode) && (wCmdLen > PH_NFCLIB_MFDF_MAXDFAPDU_SIZE)))
    {
        return PH_ERR_BUFFER_OVERFLOW;
    }

    /* Send the data to PICC */
    if (bWrappedMode)
    {
        pApdu[1] = pSendBuff[0]; /* Desfire cmd code in INS */
        pApdu[4] = (uint8_t)(wCmdLen) - 0x01u;

        memcpy(&gphNfcLib_State.bHalBufferRx[0], &pApdu[0], 5); /* PRQA S 3200 */
        memcpy(&gphNfcLib_State.bHalBufferRx[5], &pSendBuff[1], wCmdLen - 1);   /* PRQA S 3200 */
        memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen], &pApdu[2], 0x01);    /* PRQA S 3200 */

        PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_DEFAULT,
            &gphNfcLib_State.bHalBufferRx[0],
            pApdu[4] == 0x00 ? 5 : (5 + wCmdLen),
            &pRecv,
            pRxlen));

#if 0
        PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PH_NFCLIB_MFDF_WRAP_HDR_LEN,
            &pRecv,
            pRxlen));

        if (pApdu[4] != 0x00)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1,
                &pRecv,
                pRxlen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2], /* Le is always zero in wrapped mode. */
                0x01,
                &pRecv,
                pRxlen));
        }
#endif

    }
    else
    {

        PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_DEFAULT,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen));

    }
    if (bWrappedMode)
    {
        /* memcpy(pResponse, pRecv, (*pRxlen) - 2); PRQA S 3200 */
        *ppResponse = pRecv;
        statusTmp = pRecv[(*pRxlen) - 1];
        (*pRxlen) -= 2;
    }
    else
    {
        /* memcpy(pResponse, &pRecv[1], (*pRxlen) - 1);  PRQA S 3200 */
        *ppResponse = pRecv + 1;
        statusTmp = pRecv[0];
        (*pRxlen) -= 1;
    }

    if((statusTmp != PH_NFCLIB_MFDF_RESP_OPERATION_OK) && (statusTmp != PH_NFCLIB_MFDF_ISO7816_SUCCESS))
    {
        (statusTmp == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME) ? (statusTmp =  PH_ERR_SUCCESS_CHAINING) : (statusTmp = PH_ERR_PROTOCOL_ERROR);
    }
    else
    {
        statusTmp = PH_ERR_SUCCESS;
    }

    return statusTmp;

}


/**
 * This is purely internal api to send the data to PICC
 */
static phStatus_t phNfcLib_MFDF_SendDataToPICC(uint8_t * pCmd, uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * pResp, uint16_t * pRespLen )
{
    /* Utility function to send encrypted data to PICC as and when it is available from SAM */
    phStatus_t   statusTmp;
    uint8_t      bStatusByte;
    uint16_t     wIndex;
    uint16_t     wTmp;
    uint16_t     wLen;
    uint16_t     wFrameLen;
    uint8_t      pApdu[5] = { PH_NFCLIB_MFDF_WRAPPEDAPDU_CLA, 0x00, PH_NFCLIB_MFDF_WRAPPEDAPDU_P1, PH_NFCLIB_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint8_t      * pRecv;

    if (gphNfcLib_State.bWrappedMode)
    {
        wFrameLen = PH_NFCLIB_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PH_NFCLIB_MFDF_MAXDFAPDU_SIZE;
    }

    /* Send the data to PICC */
    wIndex = 0;
    wTmp = wDataLen;

    do
    {
        wLen = (wTmp < (wFrameLen - wCmdLen))? wTmp : (wFrameLen - wCmdLen);

        if (gphNfcLib_State.bWrappedMode)
        {
            pApdu[1] = pCmd[0]; /* Desfire cmd code in INS */
            pApdu[4] = (uint8_t)(wCmdLen + wLen) - 0x01u;

            memcpy(&gphNfcLib_State.bHalBufferRx[0], &pApdu[0], 5); /* PRQA S 3200 */
            memcpy(&gphNfcLib_State.bHalBufferRx[5], &pCmd[1], wCmdLen - 1);    /* PRQA S 3200 */
            memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen], &pData[wIndex], wLen);   /* PRQA S 3200 */
            memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen + wLen], &pApdu[2], 0x01); /* PRQA S 3200 */

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_DEFAULT,
                &gphNfcLib_State.bHalBufferRx[0],
                (5 + wCmdLen + wLen),
                &pRecv,
                pRespLen));

            /* To maintain crytpo the pattern of buffer_first and buffer_last */
#if 0
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_LAST,
                pApdu,
                PH_NFCLIB_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_CONT,
                &pCmd[1],
                wCmdLen - 1,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_CONT,
                &pData[wIndex],
                wLen,
                &pRecv,
                pRespLen));

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2],
                0x01,
                &pRecv,
                pRespLen));
#endif
        }
        else
        {

            memcpy(&gphNfcLib_State.bHalBufferRx[0], pCmd, wCmdLen);    /* PRQA S 3200 */
            memcpy(&gphNfcLib_State.bHalBufferRx[wCmdLen], &pData[wIndex], wLen);   /* PRQA S 3200 */

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_DEFAULT,
                &gphNfcLib_State.bHalBufferRx[0],
                wCmdLen + wLen,
                &pRecv,
                pRespLen));



#if 0
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_FIRST,
                pCmd,
                wCmdLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_LAST,
                &pData[wIndex],
                wLen,
                &pRecv,
                pRespLen));
#endif
        }
        wIndex = wIndex + wLen;
        wTmp = wTmp - wLen;

        if (gphNfcLib_State.bWrappedMode)
        {
            memcpy(pResp, pRecv, (*pRespLen) - 2); /* PRQA S 3200 */
            bStatusByte = pRecv[(*pRespLen) - 1];
            (*pRespLen) -= 2;
        }
        else
        {
            memcpy(pResp, &pRecv[1], (*pRespLen) - 1); /* PRQA S 3200 */
            bStatusByte = pRecv[0];
            (*pRespLen) -= 1;
        }

        if ((bStatusByte != PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME) &&
            (bStatusByte != PH_ERR_SUCCESS))
        {
            if(bStatusByte != PH_NFCLIB_MFDF_ISO7816_SUCCESS)
            {
                statusTmp = PH_ERR_PROTOCOL_ERROR;
            }
            else
            {
                statusTmp = PH_ERR_SUCCESS;
            }


            return statusTmp;
        }

        /* Success returned even before writing all data? protocol error */
        if ((bStatusByte == PH_ERR_SUCCESS) && (wTmp != 0))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        if(bStatusByte != 0x00)
        {
            pCmd[0] = PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME;
            wCmdLen = 1;
        }
    }
    while(wTmp);

    return bStatusByte;
}


/*
 * This api is used to plain read the mifare data, the parameter option is considered for further crypto option of mac,
 * this api is further calling get data instead of directly performing the same functionality to accomodate crypto
 */
static phStatus_t phNfcLib_MFDF_ReadData_Plain(
                                        uint8_t bOption,
                                        uint8_t * bCmdBuff,
                                        uint16_t wCmdLen,
                                        uint8_t ** ppRxdata,
                                        uint16_t * pRxdataLen
                                        )
{
    uint16_t     status = 0;

    /* Send the command */
    status = phNfcLib_MFDF_GetData(
        bCmdBuff,
        wCmdLen,
        ppRxdata,
        pRxdataLen
        );

#if 0
    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        return status;
    }

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
#endif
    return status;
}



/**
 * This api is used to get the data for read api
 * this api is entirely for non crypto mode, used for wrapped data
 */
static phStatus_t phNfcLib_MFDF_GetData(
                          uint8_t * pSendBuff,
                          uint16_t wCmdLen,
                          uint8_t ** pResponse,
                          uint16_t * pRxlen
                          )
{
    uint16_t    wOption;
    uint8_t     * pRecv;
    phStatus_t  statusTmp = 0;
    uint8_t     bStatusByte = 0xFF;
    uint8_t     bCmdBuff[10];
    uint8_t     bBackupByte = 0;
    uint16_t    wNextPos = 0;
    uint16_t    wRxBufferSize = 0;
    uint8_t     bBackupBytes[3];
    uint8_t     pApdu[5] = { PH_NFCLIB_MFDF_WRAPPEDAPDU_CLA, 0x00, PH_NFCLIB_MFDF_WRAPPEDAPDU_P1, PH_NFCLIB_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint8_t     bBackUpByte;
    uint8_t     bBackUpByte1;
    uint16_t    wBackUpLen;
    uint16_t    wTmp = 0;

    /* Status and two other bytes to be backed up before getting new frame of data */
    memset(bBackupBytes, 0x00, 3); /* PRQA S 3200 */

    (void)phhalHw_GetConfig(&gphNfcLib_Params.sPlatform.sHal,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wTmp
        );

    wOption = PH_EXCHANGE_DEFAULT;
    if (gphNfcLib_State.bWrappedMode)
    {
        if (wCmdLen > PH_NFCLIB_MFDF_MAXWRAPPEDAPDU_SIZE)
        {
            return PH_ERR_BUFFER_OVERFLOW;
        }

        pApdu[1] = pSendBuff[0];  /* Desfire command code. */
        /* Encode APDU Length*/
        pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

        memcpy(&gphNfcLib_State.bHalBufferRx[0], &pApdu[0], 5);  /* PRQA S 3200 */
        memcpy(&gphNfcLib_State.bHalBufferRx[5], &pSendBuff[1], wCmdLen - 1);  /* PRQA S 3200 */
        memcpy(&gphNfcLib_State.bHalBufferRx[4 + wCmdLen], &pApdu[2], 1);  /* PRQA S 3200 */

        statusTmp = phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_DEFAULT,
            &gphNfcLib_State.bHalBufferRx[0],
            pApdu[4] == 0x00 ? 5: (5 + wCmdLen),
            &pRecv,
            pRxlen
            );

#if 0
        statusTmp = phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PH_NFCLIB_MFDF_WRAP_HDR_LEN,
            &pRecv,
            pRxlen
            );

        if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
        {

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1,
                &pRecv,
                pRxlen
                ));

            statusTmp = phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                );
        }
#endif
        /* To handle the case where the card returns only status 91 and returns
        AF in the next frame */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            if ((gphNfcLib_State.bWrappedMode) && (*pRxlen == 2))
            {
                /* AF should always be accompanied by data. Otherwise
                it is a protocol error */
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
            }
            /* One more status byte to read from DesFire */
            bBackUpByte = pRecv[0];
            bBackUpByte1 = pRecv[1];
            wBackUpLen = *pRxlen;

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                PH_EXCHANGE_RXCHAINING,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                ));

            /* Received length can be one or two Ex: 0x91 0xAF */
            if (*pRxlen == 2)
            {
                pRecv[wBackUpLen] = pRecv[0];
                pRecv[wBackUpLen + 1] = pRecv[1];
                bStatusByte = pRecv[1];
            }
            else if (*pRxlen == 1)
            {
                bStatusByte = pRecv[0];
                pRecv[wBackUpLen] = bStatusByte;
            }
            else
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
            }

            *pRxlen = wBackUpLen + *pRxlen;

            /* Set back the backed up bytes */
            pRecv[0] = bBackUpByte;
            pRecv[1] = bBackUpByte1;
        }
        else
        {
            if (statusTmp != PH_ERR_SUCCESS)
            {
                return statusTmp;
            }
        }
    }
    else
    {
        /* Normal mode */
        if (wCmdLen > PH_NFCLIB_MFDF_MAXDFAPDU_SIZE)
        {
            return PH_ERR_BUFFER_OVERFLOW;
        }

        /* Send this on L4 */

        PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            wOption,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen
            ));
    }

    /* Storing the original pointer */
    *pResponse = pRecv;

    /* Status is 0xAF or Ox00? */
    if (*pRxlen > 0x0000)
    {
        if (gphNfcLib_State.bWrappedMode)
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1];
        }
        else
        {
            bStatusByte = (*pResponse)[wTmp];
        }
    }

    if (bStatusByte == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((gphNfcLib_State.bWrappedMode) && (*pRxlen == 2)) ||
            ((!(gphNfcLib_State.bWrappedMode)) && (*pRxlen == 1)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (gphNfcLib_State.bWrappedMode)
        {
            /* Next position will ensure overwriting on the
            SW1SW2 received from previous command */
            wNextPos = (*pRxlen) - 2;
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */
        }
        else
        {
            /* Backup the last byte */
            bBackupByte = (*pResponse)[(*pRxlen - 1)];
            memcpy(bBackupBytes, &(*pResponse)[(*pRxlen - 3)], 3); /* PRQA S 3200 */
            wNextPos = (*pRxlen) - 1;
        }

        (void)phhalHw_GetConfig(
            &gphNfcLib_Params.sPlatform.sHal,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            );
    }

    while (bStatusByte == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((gphNfcLib_State.bWrappedMode) && (*pRxlen == 2)) ||
            ((!(gphNfcLib_State.bWrappedMode)) && (*pRxlen == 1)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        if (wNextPos + PH_NFCLIB_MFDF_MAX_FRAME_SIZE >= wRxBufferSize)
        {
            /* Return 0xAF and let the caller recall the function with
            option = PH_EXCHANGE_RXCHAINING */
            /* Return the data accumulated till now and its length */
            if (gphNfcLib_State.bWrappedMode)
            {
                (*pRxlen) -= 2;
            }
            else
            {
                (*pRxlen) -= 1;
                (*pResponse)++;
            }
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        (void)phhalHw_SetConfig(&gphNfcLib_Params.sPlatform.sHal,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            );

        bCmdBuff[0] = PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME;
        wCmdLen = 1;
        if ( gphNfcLib_State.bWrappedMode )
        {
            pApdu[1] = bCmdBuff[0];  /* Desfire command code. */
            /* Encode APDU Length*/
            pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

            statusTmp = phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                PH_NFCLIB_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRxlen
                );

            if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
            {

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_CONT,
                    bCmdBuff,
                    wCmdLen,
                    &pRecv,
                    pRxlen
                    ));


                bCmdBuff[0] = 0x00; /* Le */

                statusTmp =  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_BUFFER_LAST,
                    bCmdBuff,
                    0x01,
                    &pRecv,
                    pRxlen
                    );

            }
            /* To handle the case where the card returns only status 91 and returns
            AF in the next frame */
            if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
            {
                /* One or two more status bytes to read from DesFire */
                bBackUpByte = pRecv[0];
                bBackUpByte1 = pRecv[1];
                wBackUpLen = *pRxlen;

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                    &gphNfcLib_Params.spalI14443p4,
                    PH_EXCHANGE_RXCHAINING,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    pRxlen
                    ));


                /* Received length can be one or two Ex: 0x91 0xAF */
                if (*pRxlen == 2)
                {
                    pRecv[wBackUpLen] = pRecv[0];
                    pRecv[wBackUpLen + 1] = pRecv[1];
                    bStatusByte = pRecv[1];
                }
                else if (*pRxlen == 1)
                {
                    bStatusByte = pRecv[0];
                    pRecv[wBackUpLen] = bStatusByte;
                }
                else
                {
                    return PH_ERR_PROTOCOL_ERROR;
                }

                *pRxlen = wBackUpLen + *pRxlen;

                /* Set back the backed up bytes */
                pRecv[0] = bBackUpByte;
                pRecv[1] = bBackUpByte1;
            }
            else
            {
                if (statusTmp != PH_ERR_SUCCESS)
                {
                    return statusTmp;
                }
            }
        }
        else
        {
            /* Send this on L4 */

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
                &gphNfcLib_Params.spalI14443p4,
                wOption,
                bCmdBuff,
                wCmdLen,
                &pRecv,
                pRxlen
                ));
        }

        /* Update wNextPos */
        if (gphNfcLib_State.bWrappedMode)
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1];

            /* Putback the backed up bytes */
            memcpy(&(*pResponse)[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

            wNextPos = (*pRxlen) - 2;
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */
        }
        else
        {
            bStatusByte = (*pResponse)[wNextPos];

            /* Put back the previously backedup byte */
            (*pResponse)[wNextPos] = bBackupByte;

            /* Putback the backed up bytes */
            memcpy(&(*pResponse)[wNextPos - 2], bBackupBytes, 3); /* PRQA S 3200 */

            wNextPos = (*pRxlen) - 1;
            bBackupByte = (*pResponse)[wNextPos];

            /* Backup 3 bytes. The nxt frame will overwrite these */
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 2], 3); /* PRQA S 3200 */
        }
    }
    if (gphNfcLib_State.bWrappedMode)
    {
        (*pRxlen) -= 2;
    }
    else
    {
        (*pRxlen) -= 1;
        (*pResponse)++;
    }

    if((bStatusByte != PH_NFCLIB_MFDF_RESP_OPERATION_OK) && (bStatusByte != PH_NFCLIB_MFDF_ISO7816_SUCCESS))
    {
        (bStatusByte == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME) ? (statusTmp =  PH_ERR_SUCCESS_CHAINING) : (statusTmp = PH_ERR_PROTOCOL_ERROR);
    }
    return statusTmp;
}

static phStatus_t phNfcLib_MFDF_Send7816Apdu(
                                    uint8_t bOption,
                                    uint8_t bIns,
                                    uint8_t bP1,
                                    uint8_t bP2,
                                    uint8_t bLc,
                                    uint8_t * pDataIn,
                                    uint8_t bLe,
                                    uint8_t ** ppDataOut,
                                    uint16_t *pDataLen
                                    )
{
    phStatus_t   statusTmp;
    uint8_t      bCmdBuff[20];
    uint16_t     wRxlen;
    uint16_t     wCmdLen = 0;
    uint8_t      *pRecv;

    bCmdBuff[wCmdLen++] = 0x00; /* Class is always 0x00 */
    bCmdBuff[wCmdLen++] = bIns;
    bCmdBuff[wCmdLen++] = bP1;
    bCmdBuff[wCmdLen++] = bP2;

    if (bOption & 0x01)
    {
        bCmdBuff[wCmdLen++] = bLc;
    }


    PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
        &gphNfcLib_Params.spalI14443p4,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    if ((bOption & 0x01) && (bLc > 0))
    {

        PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_BUFFER_CONT,
            pDataIn,
            bLc,
            &pRecv,
            &wRxlen
            ));
    }

    wCmdLen = 0;
    if (bOption & 0x02)
    {
        bCmdBuff[wCmdLen++] = bLe;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
        &gphNfcLib_Params.spalI14443p4,
        PH_EXCHANGE_BUFFER_LAST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    statusTmp = pRecv[wRxlen - 2];
    statusTmp <<= 8;
    statusTmp |= pRecv[wRxlen - 1];

    if((statusTmp != PH_NFCLIB_MFDF_RESP_OPERATION_OK) && (statusTmp != PH_NFCLIB_MFDF_ISO7816_SUCCESS_STATUS))
    {
        return statusTmp;
    }

    if (pDataLen != NULL)
    {
        *pDataLen = wRxlen -2;
    }

    if (ppDataOut != NULL)
    {
        *ppDataOut = pRecv;
    }

    return PH_ERR_SUCCESS;
}


/**
* This api is to perform the iso read
*/
static phStatus_t phNfcLib_MFDF_IsoRead(
                               uint16_t wOption,
                               uint8_t * bCmdBuff,
                               uint16_t wCmdLen,
                               uint8_t ** ppRxBuffer,
                               uint16_t * pBytesRead
                               )
{
    uint16_t     statusTmp;
    uint16_t     wRxBufferSize;
    uint16_t     wNextPos;
    uint8_t      *pRecv;
    uint8_t      bBackupBytes[3];

    statusTmp = phpalI14443p4_Exchange(
        &gphNfcLib_Params.spalI14443p4,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );

    /* First put everything on the reader Rx buffer upto buffer size - 60 */

    pRecv = *ppRxBuffer;

    if ((statusTmp != PH_ERR_SUCCESS) && ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        return statusTmp;
    }

    while ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        (void)phhalHw_GetConfig(
            &gphNfcLib_Params.sPlatform.sHal,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            );

        wNextPos = *pBytesRead;
        memcpy(bBackupBytes, &pRecv[wNextPos - 3], 3); /* PRQA S 3200 */

        if (wNextPos + PH_NFCLIB_MFDF_MAX_FRAME_SIZE >= wRxBufferSize)
        {
            /* Calculate partical cmac if authenticated and return PH_ERR_SUCCESS_CHAINING */
            break;
        }

        (void)phhalHw_SetConfig(
            &gphNfcLib_Params.sPlatform.sHal,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            );

        statusTmp = phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_RXCHAINING,
            bCmdBuff,
            wCmdLen,
            ppRxBuffer,
            pBytesRead
            );


        /* Put back the backed up bytes */
        memcpy(&pRecv[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

        if ((statusTmp != PH_ERR_SUCCESS) &&
            ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
        {
            return statusTmp;
        }
    }

    /* The data is now in *ppRxBuffer, length = wRxlen */

    /* Size of MAC bytes */

    if (statusTmp == PH_ERR_SUCCESS)
    {
        statusTmp = (*ppRxBuffer)[*pBytesRead - 2]; /* SW1 */
        statusTmp = statusTmp << 8; /* Shift SW1 to MSB */
        statusTmp |= (*ppRxBuffer)[*pBytesRead - 1]; /* SW2 */

        if((statusTmp != PH_NFCLIB_MFDF_RESP_OPERATION_OK) && (statusTmp != PH_NFCLIB_MFDF_ISO7816_SUCCESS_STATUS))
        {
            (statusTmp == PH_NFCLIB_MFDF_RESP_ADDITIONAL_FRAME) ? (statusTmp =  PH_ERR_SUCCESS_CHAINING) : (statusTmp = PH_ERR_PROTOCOL_ERROR);
        }
        else
        {
            statusTmp = PH_ERR_SUCCESS;
        }

        if (gphNfcLib_State.bAuthMode == PH_NFCLIB_MFDF_NOT_AUTHENTICATED)
        {
            *pBytesRead -= 2;
            return statusTmp;
        }

        if(statusTmp != PH_ERR_SUCCESS)
        {
            return statusTmp;
        }

    }
    return statusTmp;
}
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFDF */
